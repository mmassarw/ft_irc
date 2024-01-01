#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>

namespace tcp
{
    class Socket
    {
        public:
            Socket();
            virtual ~Socket() throw();
    };
} // namespace tcp

#endif