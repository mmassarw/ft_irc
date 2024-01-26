#include "Server.hpp"

//private communication between a registered user and a specific service on the IRC server, 
//allowing users to send messages or commands to services.

int Server::squery(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNumber(u, IRC::Error::norecipient(m.command())));
	if (m.params().size() == 1)
		return (writeNumber(u, IRC::Error::notexttosend()));
	const std::string &target = m.params()[0];
	User *receiver = _network.getUserByServiceName(target);
	if (!receiver)
		return (writeNumber(u, IRC::Error::nosuchservice(target)));
	receiver->sendMessage((IRC::MessageBuilder(u.label(), m.command()) << target << m.params()[1]).str());
	return (0);
}
