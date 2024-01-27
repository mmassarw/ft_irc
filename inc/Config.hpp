#pragma once

#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stdexcept>

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
		typedef std::set<std::string>				ServerHostMap;
		
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
		int 				maxMasks();
		OperatorMap 		operators();
		Operator 			*getOperator(std::string &login);
		ServerHostMap 		serverHosts();
		void				clear() throw();
		
		class ParseException : public std::exception {};

    private:
		std::string			_serverPass;
		std::string			_tcpPort;
		OperatorMap 		_operators;
		ServerHostMap 		_serverHosts;

		int checkPort(char *port);
   		int checkPass(std::string pass);
};