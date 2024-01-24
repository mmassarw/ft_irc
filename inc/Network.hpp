#pragma once

#include "Config.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Network
{
    public:
		typedef std::map<tcp::TcpSocket *, Connection *>	ConnectionMap;
		typedef std::map<std::string, User *>				UserMap;
		typedef std::map<std::string, User *>				ServiceMap;
		typedef std::map<std::string, Channel *>			ChannelMap;
		
		Network();
		~Network() throw();

		void			add(User *user);
		void			add(Channel *channel);
		void			clear() throw();
		Connection		*getConnBySocket(tcp::TcpSocket *socket);
		
	private:
		ConnectionMap	_connections;
		UserMap			_users;
		ServiceMap		_services;
		ChannelMap		_channels;
};