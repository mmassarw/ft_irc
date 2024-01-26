#include "Server.hpp"

//trigger to restart a server.

int Server::restart(User &u, const IRC::Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return writeNumber(u, IRC::Error::notregistered());
	if (!u.userMode().isSet(UserMode::OPERATOR))
		return writeNumber(u, IRC::Error::noprivileges());
	_state = RESTART;
	std::cout << "RESTARTING" << std::endl;
	return 0;
}
