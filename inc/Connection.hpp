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
		void			setType(const Type type);
		void			sendMessage(const std::string &content);

	protected:
		Connection(tcp::TcpSocket *socket, Type type);

		tcp::TcpSocket	*_socket;
		Type			_type;
};