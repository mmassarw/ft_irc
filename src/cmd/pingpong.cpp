#include "IrcServer.hpp"

//PING/PONG mechanism to maintain connection stability.

int IrcServer::ping(User &u, const IRC::Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	u.writeLine((IRC::MessageBuilder(config.servername, "PONG") << m.params()[0]).str());
	return true;
}

int IrcServer::pong(User &u, const IRC::Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	u.pongExpected() = false;
	::time(&u.clock());
	return true;
}
