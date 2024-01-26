#include "Server.hpp"
#include "libft.hpp"

//handling the registration process for services connecting to the IRC server.

int Server::service(User &u, const IRC::Message &m)
{
	if (u.isRegistered() || !u.requirements().areSet(UserRequirement::NICK | UserRequirement::USER))
		return (writeNumber(u, IRC::Error::alreadyregistred()));
	if (m.params().size() < 6)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	std::set<std::string>::const_iterator i;
	for (i = _setting.serverHosts.begin(); i != _setting.serverHosts.end(); ++i)
		if (ft::match(*i, u.socket()->host()))
			break ;
	if (i == _setting.serverHosts.end())
	{
		disconnect(u, "Access denied");
		return (-1);
	}
	const IRC::Param &nick = m.params()[0];
	if (!nick.isNickname())
		return (writeNumber(u, IRC::Error::erroneusnickname(nick)));
	if (_network.getUserByServiceName(nick) || _network.isFnick(nick))
		return (writeNumber(u, IRC::Error::nicknameinuse(nick)));
	if (!ft::match(m.params()[2], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[2])));
	if (u.requirements().isSet(UserRequirement::PASS))
	{
		writeNumber(u, IRC::Error::passwdmissmatch());
		disconnect(u, "Bad Password");
		return (-1);
	}
	_network.remove(&u);
	u.setType(User::SERVICE);
	u.setNickname(nick);
	u.setRealname(m.params()[5]);
	_network.add(&u);
	u.unsetRequirement(UserRequirement::NICK);
	u.unsetRequirement(UserRequirement::USER);
	writeWelcome(u);
	return (0);
}
