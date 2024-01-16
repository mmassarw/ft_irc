#pragma once

#include "Listener.hpp"
#include "TcpSocket.hpp"
#include <netdb.h>
#include <list>

namespace tcp
{
    class TcpServer
    {
        public:
            TcpServer();
            virtual ~TcpServer() throw();

            size_t				    getConnectionSize() const;
            void				    setMaxConnections(size_t MaxConnections);
            void				    listen(const std::string &port, size_t maxQueueLen = 5);
            void				    disconnect(TcpSocket *client) throw();
            TcpSocket			    *nextNewConnection() throw();
            TcpSocket			    *nextPendingConnection() throw();
            void				    select();

            class SigintException : public std::exception {};

        private:
            static const size_t     defaultMaxConnections;
            static const timeval    selectTimeout;

            addrinfo				_hint;
            std::list<Listener*>    _listeners;
            std::list<Listener*>    _incoming;
            std::list<TcpSocket*>	_connections;
            std::list<TcpSocket*>	_pending;
            size_t					_maxConnections;
    };
} // namespace tcp