#include "Server.hpp"
#include "libft.hpp"

//export various statistics about the server's local user population and channel count.

int Server::lusers(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[0])));
	if (m.params().size() > 1 && !ft::match(m.params()[1], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[1])));
	Network::UserMap umap = _network.users();
	Network::UserMap::const_iterator ium = umap.begin();
	writeNumber(u, IRC::Reply::lusersclient(umap.size(), _network.services().size(), 1));
	int nbOp = 0;
	int nbClients = 0;
	int nbUnknown = 0;
	while (ium != umap.end())
	{
		UserMode umode = ium->second->userMode();
		UserRequirement ureq = ium->second->requirements();
		if (umode.isSet(UserMode::OPERATOR))
			nbOp++;
		if (ureq.flags())
			nbUnknown++;
		nbClients++;
		++ium;
	}
	writeNumber(u, IRC::Reply::lusersop(nbOp));
	writeNumber(u, IRC::Reply::lusersunknown(nbUnknown));
	writeNumber(u, IRC::Reply::luserschannels(_network.channels().size()));
	writeNumber(u, IRC::Reply::lusersme(nbClients));
	return (0);
}
