#ifndef LISTENER_HPP
# define LISTENER_HPP

#include "TcpSocket.hpp"

namespace tcp
{
    class Listener : public Socket
    {
        public:
            Listener(int family);
            virtual ~Listener() throw();

            virtual     TcpSocket*	accept();
            void        listen(int maxQueuLen = 10);

        private:
            Listener();
    };
} // namespace tcp

#endif