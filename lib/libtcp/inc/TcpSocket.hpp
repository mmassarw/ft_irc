#pragma once

#include "Socket.hpp"
#include <string>

namespace tcp
{
    class TcpSocket : public Socket
    {
        public:
            virtual				~TcpSocket() throw();

            const std::string	&ip() const;
			uint16_t			port() const;
			const std::string	&host() const;

			size_t				readBufSize() const;
			bool				isLine();
			bool				canReadLine();
            bool				canWriteLine();

			bool				readLine(std::string &line);
			void				writeLine(const std::string &data) throw();
			virtual bool		recvBuffer();
            virtual void        sendBuffer();

        protected:
            TcpSocket();
            TcpSocket(int listenerFd);

            static const size_t readSize;

            std::string			_ip;
            uint16_t			_port;
            std::string			_host;
            bool				_isReadable;
            bool				_isWriteable;
            std::string			_readBuf;
            std::string			_writeBuf;
            size_t				_newline;

			virtual int			recv(void *buf, size_t size, int flags = 0);
			virtual int			send(const void *buf, size_t size, int flags = 0);
			virtual bool		isWbufEmpty() const;

		private:
			friend class Listener;
            friend class TcpServer;
    };
} // namespace tcp