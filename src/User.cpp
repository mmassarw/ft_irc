#include "User.hpp"

User::User(tcp::TcpSocket *socket) : Connection(socket, USER)
{
}

User::~User()
{
}

std::string User::nickname() const
{
    return (_nickname);
}