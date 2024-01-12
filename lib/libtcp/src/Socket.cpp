#include "Socket.hpp"
#include "logError.hpp"
#include <fcntl.h>
#include <netinet/in.h>

namespace tcp
{
    Socket::Socket() {}

    Socket::~Socket() throw() {}

    int Socket::fd() const {
        return (_fd);
    }

    const sockaddr_storage &Socket::addr() const {
        return (_addr);
    }

    sa_family_t Socket::family() const {
        return (_addr.ss_family);
    }

    void Socket::socket(int family, int protocol_num)
    {
        _fd = ::socket(family, SOCK_STREAM, protocol_num);
        if (_fd == -1)
            throw ft::logError("socket");
    }

    void Socket::bind(sockaddr *addr, socklen_t addr_size)
    {
        if (::bind(_fd, addr, addr_size) == -1)
            throw ft::logError("bind");
        std::memcpy(&_addr, &addr, addr_size);
    }

    void Socket::setReuseAddr()
    {
        int reuse = 1;
        if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
            throw ft::logError("setsockopt");
    }

    void Socket::setIpv6only()
    {
        int v6Only = 1;
        if (::setsockopt(_fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6Only, sizeof(v6Only)) == -1)
            throw ft::logError("setsockopt");
    }

    void Socket::setNonblock()
    {
        if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
            throw ft::logError("fcntl");
    }
}