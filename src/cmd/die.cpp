#include "IrcServer.hpp"

//user with operator privileges initiates a request to shut down the IRC server, affecting the entire server.

int IrcServer::die(User &u, const IRC::Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return writeNum(u, IRC::Error::notregistered());
	if (!u.umode().isSet(UserMode::OPERATOR))
		return writeNum(u, IRC::Error::noprivileges());
	_state = DIE;
	log() << "DYING" << std::endl;
	return 0;
}
