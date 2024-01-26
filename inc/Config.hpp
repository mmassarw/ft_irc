#pragma once

#include <iostream>
#include <map>

struct Operator
{
	Operator() {}
	Operator(std::string newLogin, std::string newPass, std::string newHost): login(newLogin), pass(newPass), host(newHost) {}
	
	std::string login;
	std::string pass;
	std::string host;
};

class Config
{
    public:
        typedef std::map<std::string, Operator *>	OperatorMap;
		
		Config(int ac, char **av);
        ~Config() throw();
        
        std::string 		serverName();
        std::string 		serverPass();
        std::string 		serverDesc();
        std::string 		tcpPort();
		std::string			motdfile();
        time_t      		ping();
        time_t 				pong();
        int 				maxConnections();
        int 				maxChannels();
		OperatorMap 		operators();
		Operator 			*getOperator(std::string &login);
		void				clear() throw();
		
		class ParseException : public std::exception {};

    private:
		//Config();

		std::string			_serverPass;
		std::string			_tcpPort;
		OperatorMap 		_operators;
};