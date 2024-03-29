#include "Server.hpp"
#include "libft.hpp"

// basic information about users and channels based on a specified mask.  
// o = parameter can be used to filter for operators.

int Server::who(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));

	Params pMsg = m.params();

	bool noParams = false;
	std::string mask;
	if (!pMsg.size())
	{
		noParams = true;
		mask = "";
	}
	else
		mask = pMsg[0];

	bool ope = false;
	if (pMsg.size() > 1)
		if (pMsg[1] == "o")
			ope = true;

	const Network::ChannelMap &channels = _network.channels();
	Network::ChannelMap::const_iterator chanIter = channels.begin();
	while (chanIter != channels.end() && !noParams)
	{
		Channel *c = chanIter->second;
		ChannelMode cmode = c->mode();
		if ((cmode.isSet(ChannelMode::PRIVATE)
		|| cmode.isSet(ChannelMode::SECRET)) && !c->findMember(&u))
		{
			++chanIter;
			continue;
		}

		if (ft::match(mask, c->name()))
		{
			const Channel::MemberMap &members = c->members();
			Channel::MemberMap::const_iterator memberIter = members.begin();
			while (memberIter != members.end())
			{
				User *us = memberIter->first;
				MemberMode *mmode = c->findMember(us);
				UserMode umode = us->userMode();

				if ((ope && (!umode.isSet(UserMode::OPERATOR)
				&& !mmode->isSet(UserMode::OPERATOR)))
				|| umode.isSet(UserMode::INVISIBLE))
				{
					++memberIter;
					continue;
				}

				// ---------------------------------------------- //


				std::string msg = "";

				msg += c->name() + " " + us->username();
				msg += " " + us->socket()->host();
				msg += " " + _setting.serverName + " " + us->nickname();


				if (!umode.isSet(UserMode::AWAY))
					msg += " H";
				else
					msg += " G";
				if (umode.isSet(UserMode::OPERATOR))
					msg += "*";
				if (mmode->isSet(MemberMode::OPERATOR))
					msg += "@";



				msg += " :0 " + us->realname();

				// ---------------------------------------------- //

				writeNumber(u, IRC::Reply::whoreply(msg));

				++memberIter;

			}
			writeNumber(u, IRC::Reply::endofwho(mask));
			return 0;
		}
		++chanIter;
	}

	const Network::UserMap &users = _network.users();
	Network::UserMap::const_iterator userIter = users.begin();
	while (userIter != users.end())
	{
		User *us = userIter->second;
		if (ft::match(mask, us->nickname())
		|| ft::match(mask, us->realname())
		|| ft::match(mask, us->username())
		|| ft::match(mask, _setting.serverName)
		|| noParams)
		{
			// ---------------------------------------------- //

			UserMode umode = us->userMode();

			if ((ope && !umode.isSet(UserMode::OPERATOR))
			|| umode.isSet(UserMode::INVISIBLE))
			{
				++userIter;
				continue;
			}


			std::string msg;

			msg += "* " + us->username();
			msg += " " + us->socket()->host();
			msg += " " + _setting.serverName + " " + us->nickname();

			if (!umode.isSet(UserMode::AWAY))
				msg += " H";
			else
				msg += " G";
			if (umode.isSet(UserMode::OPERATOR))
				msg += "*";

			msg += " :0 " + us->realname();

			// ---------------------------------------------- //

			writeNumber(u, IRC::Reply::whoreply(msg));
		}
		++userIter;
	}

	writeNumber(u, IRC::Reply::endofwho(mask));
	return (0);
}
