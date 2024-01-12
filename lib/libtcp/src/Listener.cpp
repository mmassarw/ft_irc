#include "Listener.hpp"
#include "logError.hpp"

namespace tcp
{
    Listener::Listener() {}

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