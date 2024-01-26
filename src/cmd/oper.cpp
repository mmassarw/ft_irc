#include "Server.hpp"
#include "libft.hpp"

//give operator privileges to a user.

int Server::oper(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	Config::OperatorMap::const_iterator io = _setting.operators.find(m.params()[0]);
	if (io == _setting.operators.end() || io->second->pass != m.params()[1])
		return (writeNumber(u, IRC::Error::passwdmissmatch()));
	if (!ft::match(io->second->host, u.label()))
		return (writeNumber(u, IRC::Error::nooperhost()));
	writeNumber(u, IRC::Reply::youreoper());
	u.setUserMode(u.userMode().flags() | UserMode::OPERATOR);
	return (0);
}
