#include "Server.hpp"

//allows an operator to forcefully terminate the connection, affecting an individual user.

int Server::kill(User &u, const IRC::Message &m)
{
	User *target;

	if (m.params().size() < 2)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	if (!u.userMode().isSet(UserMode::OPERATOR))
		return (writeNumber(u, IRC::Error::noprivileges()));
	if (!(target = _network.getUserByNickname(m.params()[0])))
		return (writeNumber(u, IRC::Error::nosuchnick(m.params()[0])));
	_network.addFnick(target->nickname());
	disconnect(*target, "killed by " + u.label() + " : " + m.params()[1]);
	return (0);
}
