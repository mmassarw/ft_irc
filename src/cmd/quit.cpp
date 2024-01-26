#include "Server.hpp"

//disconnects user from the server. 

int Server::quit(User &u, const IRC::Message &m)
{
	if (!u.isRegistered() || m.params().empty())
		disconnect(u, "Client Quit", true);
	else
		disconnect(u, m.params()[0], true);
	return (0);
}
