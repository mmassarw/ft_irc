#include "Server.hpp"
#include <iostream>

const size_t Server::maxLineSize = 2048;

Server::Server(Config &config, bool _autoInit) : _state(ACTIVE)
{
    _setting.serverName = config.serverName();
    _setting.tcpPort = config.tcpPort();
    _setting.operators = config.operators();

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
        tcp::TcpSocket *socket;
        while ((socket = _tcpSrv.nextPendingConnection()) && _state == ACTIVE)
		{
            try
            {
                socket->sendBuffer();
                std::string line;
                while (socket->canReadLine())
                {
                    if (!socket->readLine(line))
                    {
                        std::cout << "Remote host closed connection" << std::endl;
                        continue;
                    }
                    if (socket->readBufSize() > maxLineSize)
                    {
                        std::cout << "Socket's buffer size has exceed the limit" << std::endl;
                    }
                    if (line.empty())
						continue ;
                    std::cout << line;
                    exec(_network.getConnBySocket(socket), IRC::Message(line));
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cout << "Disconnected client" << std::endl;
                continue;
            }
            
        }
    }
}

void Server::writeMessage(User &user, const std::string &command, const std::string &content)
{
	user.sendMessage((IRC::MessageBuilder(_setting.serverName, command) << user.nickname() << content).str());
}

int Server::exec(Connection *sender, const IRC::Message &msg)
{
    (void)sender;
    (void)msg;
    return 0;

}