#include "TcpSocket.hpp"
#include "logError.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace tcp
{
    const size_t TcpSocket::readSize = 1024;

    TcpSocket::TcpSocket(): Socket(), _isReadable(false), _isWriteable(false), _newline(std::string::npos) {}

    TcpSocket::TcpSocket(int listenerFd): Socket(), _isReadable(false), _isWriteable(false), _newline(std::string::npos)
    {
        socklen_t addrLen = sizeof(_addr);
        if ((_fd = accept(listenerFd, (sockaddr *)&_addr, &addrLen)) == -1)
            throw ft::logError("accept");
        if (family() == AF_INET)
        {
            _port = ntohs(((sockaddr_in *)&_addr)->sin_port);
            _ip = inet_ntoa(((sockaddr_in *)&_addr)->sin_addr);
        }
        else if (family() == AF_INET6)
        {
            char buf[INET6_ADDRSTRLEN];
            _port = ntohs(((sockaddr_in6 *)&_addr)->sin6_port);
            if (!(inet_ntop(AF_INET6, &_addr, buf, INET6_ADDRSTRLEN)))
            {
                close();
                throw ft::logError("inet_ntop");
            }
            _ip = buf;
        }
        else
        {
            close();
            throw ft::logError(EPFNOSUPPORT, "accept");
        }
        char buf[NI_MAXHOST];
        if (!getnameinfo((sockaddr *)&_addr, addrLen, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
            _host = buf;
        else
            _host = _ip;
    }

    TcpSocket::~TcpSocket() throw() {
        close();
    }

    size_t TcpSocket::readBufSize() const {
        return (_readBuf.size());
    }

    bool TcpSocket::isLine()
    {
        if (_newline == std::string::npos)
            if ((_newline = _readBuf.find_first_of('\n')) == std::string::npos)
                if ((_newline = _readBuf.find_first_of('\r')) == std::string::npos)
                    return false;
        return true;
    }

    bool TcpSocket::readLine(std::string &line)
    {
        line.clear();
        if (!isLine() && !recvBuffer())
            return false;
        if (!isLine())
            return true;
        line = _readBuf.substr(0, _newline + 1);
        _readBuf.erase(0, _newline + 1);
        _newline = std::string::npos;
        return true;
    }

    bool TcpSocket::recvBuffer()
    {
        if (_isReadable)
        {
            char buf[readSize + 1];
            int nb;
            _isReadable = false;
            if (!(nb = recv(buf, readSize)))
                return false;
            buf[nb > 0 ? nb : 0] = '\0';
            _readBuf += buf;
        }
        return true;
    }

    void TcpSocket::sendBuffer()
    {
        if (!_isWriteable || isWbufEmpty())
            return ;
        _isWriteable = false;
        send(_writeBuf.c_str(), _writeBuf.size());
        _writeBuf.clear();
    }

    int TcpSocket::recv(void *buf, size_t n, int flags)
    {
        int nb;
        if ((nb = ::recv(_fd, buf, n, flags)) == -1)
            if (errno != EWOULDBLOCK)
                throw ft::logError("recv");
        return (nb);
    }

    int TcpSocket::send(const void *buf, size_t n, int flags)
    {
        int nb;
        if ((nb = ::send(_fd, buf, n, flags)) == -1)
            if (errno != EWOULDBLOCK)
                throw ft::logError("send");
        return (nb);
    }

    void TcpSocket::writeLine(const std::string &data) throw()
    {
        _writeBuf += data + '\n';
    }

    bool TcpSocket::canReadLine()
    {
        return (isLine() || _isReadable);
    }

    bool TcpSocket::canWriteLine()
    {
        return (_isWriteable);
    }

    bool TcpSocket::isWbufEmpty() const {
        return (_writeBuf.empty());
    }

    const std::string &TcpSocket::ip() const {
        return (_ip);
    }

    uint16_t TcpSocket::port() const {
        return (_port);
    }

    const std::string &TcpSocket::host() const {
        return _host;
    }
}