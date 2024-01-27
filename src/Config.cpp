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
	_serverHosts.insert("localhost");
	_serverHosts.insert("ip6-localhost");
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

std::string Config::motdfile()
{
	return ("");
}

size_t Config::historySize()
{
	return (5000);
}

int Config::maxConnections()
{
	return (5);
}

int Config::maxChannels()
{
	return (180);
}

int Config::maxMasks()
{
	return (4);
}

time_t Config::ping()
{
	return (180);
}

time_t Config::pong()
{
	return (180);
}

Config::ServerHostMap Config::serverHosts()
{
	return (_serverHosts);
}

Config::OperatorMap Config::operators()
{
	return (_operators);
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