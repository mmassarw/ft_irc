#include "Connection.hpp"

Connection::Connection(tcp::TcpSocket *socket, Type type) : _socket(socket), _type(type)
{
}

Connection::~Connection()
{
}

Connection::Type Connection::type() const
{
    return (_type);
}

tcp::TcpSocket *Connection::socket() const
{
    return (_socket);
}

void Connection::setType(const Type type)
{
	_type = type;
}

void Connection::sendMessage(const std::string &content)
{
	_socket->writeLine(content);
}