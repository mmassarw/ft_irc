#include "Server.hpp"
#include "libft.hpp"

// information about the server's version,server name ...

#define IRCSERVER_DEBUGLEVEL ""
#define IRCSERVER_COMMENTS "An obsolete IC server"

int Server::version(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[0])));
	writeNumber(u, IRC::Reply::version(_version, IRCSERVER_DEBUGLEVEL, _setting.serverName, IRCSERVER_COMMENTS));
	return (0);
}
