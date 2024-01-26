#include "Server.hpp"
#include "libft.hpp"

//handles the processing of private messages, delivers the messages accordingly.

int Server::privmsg(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::norecipient(m.command())));
	if (m.params().size() == 1)
		return (writeNumber(u, IRC::Error::notexttosend()));

	Params		targets(m.params()[0].split());
	const IRC::Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isMask())
		{
			if (!u.userMode().isSet(UserMode::OPERATOR))
			{
				writeNumber(u, IRC::Error::noprivileges());
				continue ;
			}
			const std::string mask = target->mask().substr(1);
			if ((*target)[0] == '$')
			{
				if (ft::match(mask, _setting.serverName))
					for (Network::UserMap::const_iterator i = _network.users().begin(); i != _network.users().end(); ++i)
					{
						User *v(i->second);
						if (!v->hopcount() && v->isRegistered() && v->socket() != u.socket())
							v->sendMessage((IRC::MessageBuilder(u.label(), m.command()) << *target << text).str());
					}
				else
					writeNumber(u, IRC::Error::badmask(*target));
			}
			else if ((*target)[0] == '#')
			{
				size_t dot;
				std::string toplevel;

				if ((dot = target->find_last_of('.')) == std::string::npos)
				{
					writeNumber(u, IRC::Error::notoplevel(*target));
					continue ;
				}
				toplevel = target->substr(dot);
				if (toplevel.find('*') != std::string::npos || toplevel.find('?') != std::string::npos)
					writeNumber(u, IRC::Error::wildtoplevel(*target));
				else
				{
					bool found = false;
					for (Network::UserMap::const_iterator it = _network.users().begin(); it != _network.users().end(); ++it)
						if (ft::match(mask, it->second->socket()->host()))
						{
							it->second->sendMessage((IRC::MessageBuilder(it->second->label(), m.command()) << *target << text).str());
							found = true;
						}
					if (!found)
						writeNumber(u, IRC::Error::badmask(*target));
				}
			}
		}
		else if (target->isNickname())
		{
			User *receiver = _network.getUserByNickname(*target);
			if (!receiver)
			{
				writeNumber(u, IRC::Error::nosuchnick(*target));
				continue ;
			}
			if (receiver->userMode().isSet(UserMode::AWAY))
				writeNumber(u, IRC::Reply::away(receiver->nickname(), receiver->awayReason()));
			receiver->sendMessage((IRC::MessageBuilder(u.label(), m.command()) << *target << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = _network.getChannelByName(*target);
			if (!chan)
				writeNumber(u, IRC::Error::nosuchchannel(*target));
			else if (!chan->canSendToChannel(&u))
				writeNumber(u, IRC::Error::cannotsendtochan(chan->name()));
			else
				chan->send((IRC::MessageBuilder(u.label(), m.command()) << chan->name() << text).str(), &u);
		}
	}
	u.idle() = ::time(NULL);
	return (0);
}
