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

		const ChannelMap	&channels() const;
		const ConnectionMap	&connections() const;
		const UserMap		&users() const;
		const ServiceMap	&services() const;
		void				add(User *user);
		void				add(Channel *channel);
		void				clear() throw();
		Connection			*getConnBySocket(tcp::TcpSocket *socket);
		User				*getUserByNickname(const std::string &nickname);
		User				*getUserByServiceName(const std::string &serviceName);
		Channel				*getChannelByName(const std::string &channelName);
		void				remove(User *user) throw();
		void				remove(const Channel *chan) throw();
		void				resetUserReceipt();
		
	private:
		ConnectionMap	_connections;
		UserMap			_users;
		ServiceMap		_services;
		ChannelMap		_channels;
};