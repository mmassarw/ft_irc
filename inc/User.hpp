#pragma once

#include "Connection.hpp"

class User : public Connection
{
    public:
        User(tcp::TcpSocket *socket);
        ~User();

        std::string         nickname() const;

    private:
        std::string         _nickname;
};