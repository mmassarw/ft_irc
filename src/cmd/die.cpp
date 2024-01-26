#include "Server.hpp"

//user with operator privileges initiates a request to shut down the IRC server, affecting the entire server.

int Server::die(User &u, const IRC::Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return writeNumber(u, IRC::Error::notregistered());
	if (!u.userMode().isSet(UserMode::OPERATOR))
		return writeNumber(u, IRC::Error::noprivileges());
	_state = SHUTDOWN;
	std::cout << "DYING" << std::endl;
	return 0;
}
