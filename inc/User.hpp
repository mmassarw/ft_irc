#pragma once

#include "Connection.hpp"
#include "UserMode.hpp"
#include "UserRequirement.hpp"

class User : public Connection
{
    public:
        User(tcp::TcpSocket *socket, UserRequirement::Flag requirements);
        ~User();

		bool					isRegistered() const;
		time_t					&idle();
		void					mark();
		void					unmark();
		void					setRequirement(UserRequirement::Flag requirement);
		void					unsetRequirement(UserRequirement::Flag requirement);
		const UserRequirement	&requirements() const;
		const std::string		&realname() const;
		const UserMode			&userMode() const;
		size_t					joinedChannels() const;
		const std::string		&awayReason() const;
		const std::string 		&label() const;
        const std::string       &nickname() const;
        const std::string		&username() const;
		void					setRealname(const std::string &realname);
		void					setUserMode(const UserMode &userMode);
		void					joinedChannels(size_t joinedChannels);
		void					setAway(const std::string &reason);
		void					setNickname(const std::string &nickname);
		void					setUsername(const std::string &username);

	protected:
		UserRequirement			_requirements;
		std::string				_realname;
		UserMode				_userMode;
		size_t					_joinedChannels;
		std::string				_awayReason;
		time_t					_idle;

    private:
		std::string         	_label;
        std::string         	_nickname;
        std::string		    	_username;

		void					updateLabel();
};