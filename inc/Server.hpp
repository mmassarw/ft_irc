#pragma once
#include "TcpServer.hpp"
#include "Config.hpp"

class Server
{
    public:
        Server(Config &config);
        ~Server();

        enum State
        {
            ACTIVE,
            RESTART,
            SHUTDOWN
        };

        State			state() const;
        void            run();
        void            loadConfig();

    private:
        Server();
        
        State			_state;
        time_t			_creation;
        tcp::TcpServer	_tcpSrv;
        Config          _config;
};