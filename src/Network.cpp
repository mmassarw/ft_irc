#include "Network.hpp"

Network::Network()
{
}

Network::~Network() throw()
{
    clear();
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