#pragma once

#include <sys/socket.h>

namespace tcp
{
    class Socket
    {
        public:
            Socket();
            virtual ~Socket() throw();

            int                     fd() const;
            const sockaddr_storage  &addr() const;
            sa_family_t             family() const;
            void                    socket(int family, int protocol_num);
            int                     close() throw();
            void                    bind(sockaddr *addr, socklen_t addr_size);
            void                    setReuseAddr();
            void                    setIpv6only();
            void                    setNonblock();

        protected:
            int                     _fd;
            sockaddr_storage        _addr;
    };
} // namespace tcp