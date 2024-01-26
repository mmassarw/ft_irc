#pragma once

#include "Config.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <map>
#include <list>
#include <vector>

struct UserInfo
{
	std::string nickname;
	std::string username;
	std::string host;
	std::string realname;
	UserInfo(const User &user);
};

class Network
{
    public:
		typedef std::map<tcp::TcpSocket *, Connection *>	ConnectionMap;
		typedef std::map<std::string, User *>				UserMap;
		typedef std::map<std::string, User *>				ServiceMap;
		typedef std::map<std::string, Channel *>			ChannelMap;

		typedef std::set<std::string>						FnicksSet;
		typedef std::vector<UserInfo>						InfoVec;
		typedef std::list<UserInfo>							HistoryList;
		
		Network();
		~Network() throw();

		const ChannelMap	&channels() const;
		const ConnectionMap	&connections() const;
		const UserMap		&users() const;
		const ServiceMap	&services() const;
		FnicksSet			&fnicks();
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
		bool				isFnick(const std::string &nick);
		void				addFnick(const std::string &nick);
		void				addNickToHistory(const User &user);
		InfoVec				getNickHistory(const std::string &nick, size_t count);
		void				setHistorySize(size_t size);
		
	private:
		ConnectionMap	_connections;
		UserMap			_users;
		ServiceMap		_services;
		ChannelMap		_channels;
		FnicksSet		_fnicks;
		HistoryList		_history;
		size_t			_historySize;
};