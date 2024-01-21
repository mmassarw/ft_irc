#include "Server.hpp"
#include <iostream>

Server::Server(Config &config, bool _autoInit) : _state(ACTIVE)
{
    _setting.serverName = config.serverName();
    _setting.tcpPort = config.tcpPort();
    _setting.maxConnections = config.maxConnections();

    _tcpSrv.listen(_setting.tcpPort.c_str());
    if (_autoInit)
        init();
}

Server::~Server()
{
}

void Server::init()
{
    _tcpSrv.setMaxConnections(_setting.maxConnections);
}

Server::State Server::state() const
{
    return (_state);
}

void Server::run()
{
    while (_state == ACTIVE)
	{
        try {
            _tcpSrv.select();
        } catch (tcp::TcpServer::SigintException &e) {
            std::cout << std::endl << "Server Closed!" << std::endl;
            _state = SHUTDOWN;
            return ;
        }

        tcp::TcpSocket *newSocket;
        while ((newSocket = _tcpSrv.nextNewConnection()))
		{
			User *user = new User(newSocket);
			writeMessage(*user, "NOTICE", "Connection established");
			std::cout << user->socket()->host() << " CONNECTED" << std::endl;
			_network.add(user);
		}
    }
}

void Server::writeMessage(User &user, const std::string &command, const std::string &content)
{
	user.sendMessage((IRC::MessageBuilder(_setting.serverName, command) << user.nickname() << content).str());
}