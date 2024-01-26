#include "Server.hpp"

//leave one or more IRC channels.

int Server::part(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	Params channels(m.params()[0].split());

	for (Params::const_iterator chan(channels.begin()); chan != channels.end(); ++chan)
	{
		Channel *c;
		if (!chan->isChannel() || !(c = _network.getChannelByName(*chan)))
			writeNumber(u, IRC::Error::nosuchchannel(*chan));
		else if (!c->findMember(&u))
			writeNumber(u, IRC::Error::notonchannel(*chan));
		else
		{
			IRC::MessageBuilder msg(u.label(), m.command());
			msg << *chan;
			if (m.params().size() > 1)
				msg << m.params()[1];
			c->send(msg.str());
			c->delMember(&u);
			if (!c->count())
				_network.remove(c);
		}
	}
	return (0);
}
