#include "Server.hpp"
#include "libft.hpp"

// sends information about visible channels, considering user membership and channel modes, 
// and responds to the user with details about the channels, including the number of visible users and the channel topic.

int Server::list(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() > 1 && !ft::match(m.params()[1], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[1])));
	if (m.params().empty())
	{
		const Network::ChannelMap channels = _network.channels();
		for (Network::ChannelMap::const_iterator i = channels.begin(); i != channels.end(); ++i)
		{
			Channel *c = i->second;
			if (!c->mode().isSet(ChannelMode::PRIVATE | ChannelMode::SECRET) || c->findMember(&u))
				writeNumber(u, IRC::Reply::list(c->name(), c->nbUserVisible(), c->topic()));
		}
	}
	else
	{
		Params args = m.params()[0].split();
		Channel *c;
		for (Params::const_iterator i = args.begin(); i != args.end(); ++i)
			if (i->isChannel() && (c = _network.getChannelByName(*i)) && (!c->mode().isSet(ChannelMode::SECRET) || c->findMember(&u)))
				writeNumber(u, IRC::Reply::list(c->name(), c->nbUserVisible(), c->topic()));
			else
				writeNumber(u, IRC::Error::nosuchchannel(*i));
	}
	writeNumber(u, IRC::Reply::listend());
	return (0);
}
