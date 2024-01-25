#include "User.hpp"

User::User(tcp::TcpSocket *socket, UserRequirement::Flag requirements) : Connection(socket, USER), _requirements(requirements), _joinedChannels(0)
{
}

User::~User()
{
}

bool User::isRegistered() const
{
	return (!_requirements.flags());
}

void User::mark()
{
    _userMode.set(UserMode::MARK);
}

void User::unmark()
{
    _userMode.unset(UserMode::MARK);
}

void User::setRequirement(UserRequirement::Flag requirement)
{
	_requirements.set(requirement);
}

void User::unsetRequirement(UserRequirement::Flag requirement)
{
	_requirements.unset(requirement);
}

const UserRequirement &User::requirements() const
{
	return (_requirements);
}

const std::string &User::realname() const
{
	return (_realname);
}

const UserMode &User::userMode() const
{
	return (_userMode);
}

size_t User::joinedChannels() const
{
	return (_joinedChannels);
}

const std::string &User::awayReason() const
{
	return (_awayReason);
}

const std::string &User::label() const
{
	return (_label);
}

const std::string &User::nickname() const
{
    return (_nickname);
}

const std::string &User::username() const
{
    return (_username);
}

void User::setRealname(const std::string &realname)
{
	_realname = realname;
}

void User::setUserMode(const UserMode &userMode)
{
    _userMode = userMode;
}

void User::joinedChannels(size_t joinedChannels)
{
    _joinedChannels = joinedChannels;
}

void User::setAway(const std::string &reason)
{
	_awayReason = reason;
}

void User::setNickname(const std::string &nickname)
{
    _nickname = nickname;
    updateLabel();
}

void User::setUsername(const std::string &username)
{
    _username = username;
    updateLabel();
}

void User::updateLabel()
{
	_label = _nickname;
	if (_username.size())
		_label += "!" + _username;
	_label += "@" + _socket->host();
}