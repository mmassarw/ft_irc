#include "Config.hpp"

Config::Config(int ac, char **av)
{
    if (ac != 3)
        throw ParseException();
    _tcpPort = av[1];
    _serverPass = av[2];
}

Config::~Config()
{
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
