#include "Server.hpp"
#include "libft.hpp"

//provides the user with the current time information of the IRC server, optionally specifying a target server.

int Server::time(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[0])));
	return (writeNumber(u, IRC::Reply::time(_setting.serverName)));
}
