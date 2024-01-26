#include "Server.hpp"

//status that user may not respond immediately.

int Server::away(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().empty())
	{
		writeNumber(u, IRC::Reply::unaway());
		u.setUserMode(UserMode(u.userMode().flags() & ~UserMode::AWAY));
	}
	else
	{
		writeNumber(u, IRC::Reply::nowaway());
		u.setUserMode(UserMode(u.userMode().flags() | UserMode::AWAY));
		u.setAway(m.params()[0]);
	}
	return (0);
}