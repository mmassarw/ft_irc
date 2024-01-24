#pragma once

#include "TcpServer.hpp"
#include "Config.hpp"
#include "IRC.hpp"
#include "User.hpp"
#include "Network.hpp"

struct ServerSetting
{
    ServerSetting() {}
    ServerSetting(std::string newServerName, std::string newTcpPort, int newMaxConnections) : serverName(newServerName), tcpPort(newTcpPort), maxConnections(newMaxConnections) {}

    std::string             serverName;
    std::string             tcpPort;
    int                     maxConnections;
    Config::OperatorMap     operators;
};

class Server
{
    public:
        Server(Config &config, bool _autoInit = true);
        ~Server();

        enum State
        {
            ACTIVE,
            RESTART,
            SHUTDOWN
        };

        State			state() const;
        void            init();
        void            run();

    private:
        Server();

        static const size_t maxLineSize;
        
        State			_state;
        time_t			_creation;
        tcp::TcpServer	_tcpSrv;
        ServerSetting   _setting;
        Network			_network;

        void	        writeMessage(User &user, const std::string &command, const std::string &content);
        int             exec(Connection *sender, const IRC::Message &msg);
};