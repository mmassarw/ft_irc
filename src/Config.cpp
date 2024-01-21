#include "Config.hpp"
#include <iostream>

Config::Config(int ac, char **av)
{
    if (ac != 3)
        throw ParseException();
    _tcpPort = av[1];
    _serverPass = av[2];
    if (!_tcpPort.size() || !_serverPass.size())
        throw ParseException();
    
    _operators["admin"] = new Operator("admin", "admin123", "*@localhost");
}

Config::~Config() throw()
{
    clear();
}

std::string Config::serverName()
{
	return ("irc.fmj.42");
}

std::string Config::serverPass()
{
	return (_serverPass);
}

std::string Config::serverDesc()
{
	return ("Welcome to FMJ server!");
}

std::string Config::tcpPort()
{
	return (_tcpPort);
}

time_t Config::ping()
{
	return (180);
}

time_t Config::pong()
{
	return (180);
}

int Config::maxConnections()
{
	return (5);
}

int Config::maxChannels()
{
	return (180);
}

Operator *Config::getOperator(std::string &login)
{
	return (_operators[login]);
}

void Config::clear() throw()
{
    for (OperatorMap::iterator i = _operators.begin(); i != _operators.end(); ++i)
		delete i->second;
}