#include "Config.hpp"
#include <iostream>

Config::Config(int ac, char **av)
{
    if (ac != 3)
        throw ParseException();

	 try {
        checkPort(av[1]); // Validate port
        checkPass(av[2]); // Validate password
    } catch (const std::exception& e) {
        std::cerr << "Error in constructor: " << e.what() << std::endl;
        throw ParseException();
    }

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

int Config::checkPort(char *port)
{
    char *endptr;
    long portNbr;
    if (port == NULL || strlen(port) == 0)
    {
        throw std::invalid_argument("ERROR! Port argument is Empty!\n");
    }

    errno = 0; // Reset errno before calling strtol
    portNbr = std::strtol(port, &endptr, 10);
    if (endptr == port)
    {
        throw std::invalid_argument("ERROR! No digits were found.\n");
    }
    if (errno == ERANGE || portNbr < 1024 || portNbr > 65535)
    {
        throw std::out_of_range("ERROR! Port number should be in the range 1024-65535.\n");
    }
    if (*endptr != '\0')
    {
        throw std::invalid_argument("ERROR! Port argument contains non-numeric characters.\n");
    }
    return 0;
}

int Config::checkPass(std::string pass)
{
    if (pass.empty())
    {
        throw std::invalid_argument("ERROR! Password is empty.\n");
    }
    for (std::size_t i = 0; i < pass.length(); ++i)
    {
        if (std::isspace(static_cast<unsigned char>(pass[i])))
        {
            throw std::invalid_argument("ERROR! Password with spaces.\n");
        }
    }
    return 0;
}