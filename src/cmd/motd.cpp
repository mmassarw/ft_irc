#include "IrcServer.hpp"

//Message of the Day, allows to request and receive a text message displayed by the server 
//upon connection, providing information, announcements ...
int IrcServer::motd(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() > 0 && m.params()[0] != config.servername)
		writeNum(u, IRC::Error::nosuchserver(m.params()[0]));
	else
		writeMotd(u);
	return (0);
}
