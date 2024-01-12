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

            int fd() const;
            const sockaddr_storage &addr() const;
            sa_family_t family() const;
            void socket(int family);
            int close() throw();
            void bind(sockaddr *addr, socklen_t addr_size);
    };
} // namespace tcp

#endif