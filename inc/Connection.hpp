#pragma once

#include "TcpSocket.hpp"

class Connection
{
    public:
        virtual ~Connection();

		enum Type
		{
			SERVICE,
			USER
		};

		Type			type() const;
		tcp::TcpSocket	*socket() const;
		unsigned		hopcount() const;
		void			setType(const Type type);
		void			sendMessage(const std::string &content);

	protected:
		Connection(tcp::TcpSocket *socket, Type type, unsigned hopcount = 0);

		tcp::TcpSocket	*_socket;
		Type			_type;
		unsigned		_hopcount;
};