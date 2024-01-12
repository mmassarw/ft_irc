#include "TcpSocket.hpp"
#include "logError.hpp"
#include <fcntl.h>
#include <netinet/in.h>

namespace tcp
{
    const size_t TcpSocket::readSize = 1024;

    TcpSocket::TcpSocket():Socket(), _isReadable(false), _isWriteable(false), _newline(std::string::npos) {}

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
        if (!isLine())
        {
            if (!fill())
                return false;
            return true;
        }   
        line = _readBuf.substr(0, _newline + 1);
        _readBuf.erase(0, _newline + 1);
        _newline = std::string::npos;
        return (true);
    }

    bool TcpSocket::fill()
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