#include "Server.hpp"

//PING/PONG mechanism to maintain connection stability.

int Server::ping(User &u, const IRC::Message &m)
{
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	u.sendMessage((IRC::MessageBuilder(_setting.serverName, "PONG") << m.params()[0]).str());
	return true;
}

int Server::pong(User &u, const IRC::Message &m)
{
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	u.pongExpected() = false;
	::time(&u.clock());
	return true;
}
