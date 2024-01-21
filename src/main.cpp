#include "Server.hpp"
#include "Config.hpp"

#include <iostream>

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    Server::State state = Server::ACTIVE;
    while (state != Server::SHUTDOWN)
    {;
        try {
            Config config = Config(ac, av);
            Server server = Server(config);
            
            server.run();
            state = server.state();
        } catch (Config::ParseException &e) {
            std::cout << "usage: ./ircserv <port> <password>" << std::endl;
            return (1);
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            return (1);
        }
    }
    return (0);
}