#pragma once
#include <iostream>
#include <map>

struct Operator
{
	Operator();
	Operator(std::string login, std::string pass, std::string host);
	
	std::string login;
	std::string pass;
	std::string host;
};

class Config
{
    public:
        Config(int ac, char **av);
        ~Config();
        
        std::string 							serverName();
        std::string 							serverPass();
        std::string 							serverDesc();
        std::string 							tcpPort();
        time_t      							ping();
        time_t 									pong();
        int 									maxConnections();
        int 									maxChannels();
		Operator 								oper(std::string &login);
		
		class ParseException : public std::exception {};

    private:
		Config();
		
		std::string								_serverPass;
		std::string								_tcpPort;
		std::map<std::string, Operator> 		_opers;
};