#include "Server.hpp"
#include <iostream>

Server::Server(Config &config) : _config(config)
{
    _tcpSrv.listen(config.tcpPort().c_str());
    loadConfig();
}

Server::~Server()
{
}

void Server::loadConfig()
{
    _tcpSrv.setMaxConnections(_config.maxConnections());
}

Server::State Server::state() const
{
    return (_state);
}

void Server::run()
{
    _state = SHUTDOWN;
}