#include "Server.hpp"

//handles the removal of users from one or more channels by a channel operator or creator

int Server::kick(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	Params chans = m.params()[0].split(), users = m.params()[1].split();
	if (chans.size() > 1 && chans.size() != users.size())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	Params::const_iterator i = users.begin();
	for (Params::const_iterator j = chans.begin(); j != chans.end(); ++j)
	{
		Channel *c = _network.getChannelByName(*j);
		if (!c)
		{
			writeNumber(u, IRC::Error::nosuchchannel(*j));
			continue ;
		}
		if (c->mode().isSet(Channel::UNMODERATED))
		{
			writeNumber(u, IRC::Error::nochanmodes(*j));
			continue;
		}
		MemberMode *mmode = c->findMember(&u);
		if (!mmode)
		{
			writeNumber(u, IRC::Error::notonchannel(c->name()));
			continue;
		}
		if (!mmode->isSet(MemberMode::OPERATOR) && !mmode->isSet(MemberMode::CREATOR))
		{
			writeNumber(u, IRC::Error::chanoprisneeded(c->name()));
			continue;
		}
		for (; i != users.end(); ++i)
		{
			User *ufind = _network.getUserByNickname(*i);
			if (ufind && c->findMember(ufind))
			{
				IRC::MessageBuilder buf(u.label(), m.command());
				buf << c->name() << ufind->nickname();
				if (m.params().size() > 2)
					buf << m.params()[2];
				else
					buf << "no reason";
				c->send(buf.str());
				c->delMember(ufind);
			}
			else
				writeNumber(u, IRC::Error::usernotinchannel(*i, c->name()));
			if (chans.size() != 1)
				break ;
		}
		if (!c->count())
			_network.remove(c);
	}
	return (0);
}
