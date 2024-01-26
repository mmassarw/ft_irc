#include "Server.hpp"

//information about the online status and host information of up to five users.

int Server::userhost(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	for (size_t i = 0; i < std::min(m.params().size(), 5UL); ++i)
	{
		User *us = _network.getUserByNickname(m.params()[i]);
		if (!us)
			continue ;
		UserMode umode = us->userMode();
		std::string msg;
		msg += m.params()[i];
		msg += (umode.isSet(UserMode::OPERATOR) ? "*=" : "=");
		msg += (umode.isSet(UserMode::AWAY) ? "-" : "+");
		msg += us->username() + "@" + us->socket()->host();
		writeNumber(u, IRC::Reply::userhostreply(msg));
	}
	return (0);
}
