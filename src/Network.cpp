#include "Network.hpp"

Network::Network()
{
}

Network::~Network() throw()
{
    clear();
}

const Network::ChannelMap &Network::channels() const
{
	return (_channels);
}
const Network::ConnectionMap &Network::connections() const
{
	return (_connections);
}

const Network::UserMap &Network::users() const
{
	return (_users);
}

const Network::ServiceMap &Network::services() const
{
	return (_services);
}

void Network::add(User *user)
{
    _connections[user->socket()] = user;
	if (user->nickname().size())
		_users[user->nickname()] = user;
}

void Network::add(Channel *channel)
{
    _channels[channel->name()] = channel;
}

void Network::clear() throw()
{
    for (UserMap::iterator i = _users.begin(); i != _users.end(); ++i)
		delete i->second;
	for (ChannelMap::iterator i = _channels.begin(); i != _channels.end(); ++i)
		delete i->second;
	for (ServiceMap::iterator i = _services.begin(); i != _services.end(); ++i)
		delete i->second;
}

Connection *Network::getConnBySocket(tcp::TcpSocket *socket)
{
	ConnectionMap::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

User *Network::getUserByNickname(const std::string &nickname)
{
	UserMap::const_iterator i = _users.find(nickname);
	return (i == _users.end() ? NULL : i->second);
}

User *Network::getUserByServiceName(const std::string &serviceName)
{
	ServiceMap::const_iterator i = _services.find(serviceName);
	return (i == _services.end() ? NULL : i->second);
}

Channel *Network::getChannelByName(const std::string &channelName)
{
	ChannelMap::const_iterator i = _channels.find(channelName);
	return (i == _channels.end() ? NULL : i->second);
}

void Network::remove(User *user) throw()
{
	if (!user->hopcount())
		_connections.erase(user->socket());
	if (user->type() == User::SERVICE)
		_services.erase(user->nickname());
	else
		_users.erase(user->nickname());
}

void Network::remove(const Channel *channel) throw()
{
	_channels.erase(channel->name());
	delete channel;
}

void Network::resetUserReceipt()
{
	for (UserMap::const_iterator i = _users.begin(); i != _users.end(); ++i)
		i->second->unmark();
}