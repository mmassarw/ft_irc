#include "Connection.hpp"

Connection::Connection(tcp::TcpSocket *socket, Type type, unsigned hopcount) :
_socket(socket), _type(type), _hopcount(hopcount), _clock(time(NULL)), _pongExpected(false)
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

unsigned Connection::hopcount() const
{
	return (_hopcount);
}

void Connection::setType(const Type type)
{
	_type = type;
}

void Connection::sendMessage(const std::string &content)
{
	_socket->writeLine(content);
}

time_t &Connection::clock()
{
	return _clock;
}

bool &Connection::pongExpected()
{
	return _pongExpected;
}