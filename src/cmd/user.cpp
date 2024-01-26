#include "Server.hpp"

//providing essential user information to the server, including a username, modes, and realname.
//The server registers the user if needed.

int Server::user(User &u, const IRC::Message &m)
{
	if (!u.requirements().isSet(UserRequirement::USER))
		return (writeNumber(u, IRC::Error::alreadyregistred()));
	else if (m.params().size() < 4)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	u.setUsername(m.params()[0]);
	u.setUserMode(strtoul(m.params()[1].c_str(), NULL, 10) & (UserMode::INVISIBLE | UserMode::WALLOPS | UserMode::RESTRICTED));
	u.setRealname(m.params()[3]);
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
	{
		writeWelcome(u);
		_network.addNickToHistory(u);
	}
	else if (u.requirements().flags() == UserRequirement::PASS)
	{
		writeNumber(u, IRC::Error::passwdmissmatch());
		disconnect(u, "Bad Password");
	}
	return (0);
}
