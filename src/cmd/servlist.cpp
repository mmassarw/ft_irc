#include "Server.hpp"
#include "libft.hpp"

//list of services available on the server.

#define IRC_WILDCARD "*"

int Server::servlist(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	std::string mask;
	if (m.params().size())
		mask = m.params()[0].mask();
	const Network::ServiceMap &map = _network.services();
	for (Network::ServiceMap::const_iterator i = map.begin(); i != map.end(); ++i)
	{
		const User *s = i->second;
		if ((mask.size() && !ft::match(mask, s->nickname())))
			continue ;
		writeNumber(u, IRC::Reply::servlist(s->nickname(), _setting.serverName, IRC_WILDCARD, IRC_WILDCARD, s->hopcount(), s->realname()));
	}
	writeNumber(u, IRC::Reply::servlistend((mask.empty() ? IRC_WILDCARD : mask), IRC_WILDCARD));
	return (0);
}
