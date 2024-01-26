#include "Server.hpp"
#include "libft.hpp"

//historical information about users who were previously connected to the IRC network but are not currently online.

int Server::whowas(User &sender, const IRC::Message &m)
{
	if (!sender.isRegistered())
		return (writeNumber(sender, IRC::Error::notregistered()));
	if (m.params().size() == 0)
		return (writeNumber(sender, IRC::Error::nonicknamegiven()));
	Params targets = m.params()[0].split(5);
	int count = 0;
	if (m.params().size() > 1)
		count = atoi(m.params()[1].c_str());
	if (m.params().size() > 2)
		if (!ft::match(m.params()[2].mask(), _setting.serverName))
			return writeNumber(sender, IRC::Error::nosuchserver(m.params()[2]));
	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		Network::InfoVec h = _network.getNickHistory(*target, count);
		if (!h.size())
			writeNumber(sender, IRC::Error::wasnosuchnick(*target));
		else
			for (size_t i = 0; i < h.size(); ++i)
			{
				writeNumber(sender, IRC::Reply::whowasuser(*target, h[i].username, h[i].host, h[i].realname));
				writeNumber(sender, IRC::Reply::whoisserver(*target, _setting.serverName, _setting.serverDesc));
			}
	}
	return writeNumber(sender, IRC::Reply::endofwhowas(sender.nickname()));
}
