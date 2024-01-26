#include "Server.hpp"

//set or unset a password for a user.

int Server::pass(User &u, const IRC::Message &m)
{
	if (u.isRegistered())
		return writeNumber(u, IRC::Error::alreadyregistred());
	if (!m.params().size())
		return writeNumber(u, IRC::Error::needmoreparams(m.command()));
	if (!_setting.serverPass.size() || m.params()[0] == _setting.serverPass)
		u.unsetRequirement(UserRequirement::PASS);
	else
		u.setRequirement(UserRequirement::PASS);
	return 0;
}
