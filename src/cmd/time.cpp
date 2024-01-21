#include "Server.hpp"
#include "libft.hpp"

//provides the user with the current time information of the IRC server, optionally specifying a target server.

int IrcServer::time(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));
	return (writeNum(u, IRC::Reply::time(config.servername)));
}
