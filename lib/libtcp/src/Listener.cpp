#include "Listener.hpp"
#include "logError.hpp"
#include <netinet/in.h>

namespace tcp
{
    Listener::Listener(int family): Socket() {
        socket(family, IPPROTO_TCP);
    }

    Listener::~Listener() throw() {
        close();
    }

    TcpSocket* Listener::accept() {
        return (new TcpSocket(_fd));
    }

    void Listener::listen(int maxQueueLen)
    {
        if (::listen(_fd, maxQueueLen) == -1)
            throw ft::logError("listen");
    }
}