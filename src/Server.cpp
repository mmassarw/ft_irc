#include "Server.hpp"
#include "libft.hpp"
#include <iostream>

CommandStats::CommandStats() : count(0), byteCount(0)
{}

const std::string Server::_version = "1.0.0";
const size_t Server::maxLineSize = 2048;

Server::Server(Config &config, bool _autoInit) : _state(ACTIVE), _creation(::time(NULL))
{
    _creationDate = ft::toDate(_creation, "%a %b %d %Y at %H:%M:%S %Z");
    
    _setting.serverName = config.serverName();
    _setting.serverPass = config.serverPass();
	_setting.serverDesc = config.serverDesc();
    _setting.tcpPort = config.tcpPort();
    _setting.operators = config.operators();
    _setting.maxConnections = config.maxConnections();
    _setting.maxChannels = config.maxChannels();
    _setting.maxMasks = config.maxMasks();
	_setting.ping = config.ping();
	_setting.pong = config.pong();
	_setting.serverHosts = config.serverHosts();
    if (config.motdfile().size())
		ft::fileToData(config.motdfile(), _setting.motd, 80);
    
    _commands[User::SERVICE]["KILL"] = &Server::kill;
	_commands[User::SERVICE]["NICK"] = &Server::nick;
	_commands[User::SERVICE]["NOTICE"] = &Server::notice;
	_commands[User::SERVICE]["OPER"] = &Server::oper;
	_commands[User::SERVICE]["PASS"] = &Server::pass;
	_commands[User::SERVICE]["PING"] = &Server::ping;
	_commands[User::SERVICE]["PONG"] = &Server::pong;
	_commands[User::SERVICE]["PRIVMSG"] = &Server::privmsg;
	_commands[User::SERVICE]["QUIT"] = &Server::quit;
	_commands[User::SERVICE]["SERVICE"] = &Server::service;
	_commands[User::SERVICE]["SERVLIST"] = &Server::servlist;
	_commands[User::SERVICE]["SQUERY"] = &Server::squery;
	_commands[User::SERVICE]["USER"] = &Server::user;
	_commands[User::SERVICE]["WHO"] = &Server::who;
	_commands[User::SERVICE]["WHOIS"] = &Server::whois;
	_commands[User::SERVICE]["WHOWAS"] = &Server::whowas;
	_commands[User::USER]["AWAY"] = &Server::away;
	_commands[User::USER]["DIE"] = &Server::die;
	_commands[User::USER]["INFO"] = &Server::info;
	_commands[User::USER]["INVITE"] = &Server::invite;
	_commands[User::USER]["JOIN"] = &Server::join;
	_commands[User::USER]["KICK"] = &Server::kick;
	_commands[User::USER]["LIST"] = &Server::list;
	_commands[User::USER]["LUSERS"] = &Server::lusers;
	_commands[User::USER]["MODE"] = &Server::mode;
	_commands[User::USER]["MOTD"] = &Server::motd;
	_commands[User::USER]["NAMES"] = &Server::names;
	_commands[User::USER]["PART"] = &Server::part;
	_commands[User::USER]["RESTART"] = &Server::restart;
	_commands[User::USER]["STATS"] = &Server::stats;
	_commands[User::USER]["TIME"] = &Server::time;
	_commands[User::USER]["TOPIC"] = &Server::topic;
	_commands[User::USER]["USERHOST"] = &Server::userhost;
	_commands[User::USER]["VERSION"] = &Server::version;

    _tcpSrv.listen(_setting.tcpPort.c_str());
    if (_autoInit)
        init();
}

Server::~Server()
{
}

void Server::init()
{
    _tcpSrv.setMaxConnections(_setting.maxConnections);
}

Server::State Server::state() const
{
    return (_state);
}

void Server::run()
{
    while (_state == ACTIVE)
	{
        try {
            _tcpSrv.select();
        } catch (tcp::TcpServer::SigintException &e) {
            std::cout << std::endl << "Server Closed!" << std::endl;
            _state = SHUTDOWN;
            return ;
        }
		closeLostConnections();
        tcp::TcpSocket *newSocket;
        while ((newSocket = _tcpSrv.nextNewConnection()))
		{
			User *user = new User(newSocket, _setting.serverPass.size() ? UserRequirement::ALL : UserRequirement::ALL_EXCEPT_PASS);
			writeMessage(*user, "NOTICE", "Connection established");
			std::cout << user->socket()->host() << " CONNECTED" << std::endl;
			_network.add(user);
		}
        tcp::TcpSocket *socket;
        while ((socket = _tcpSrv.nextPendingConnection()) && _state == ACTIVE)
		{
            try
            {
                socket->sendBuffer();
                std::string line;
                while (socket->canReadLine())
                {
                    if (!socket->readLine(line))
                    {
                        disconnect(socket, "Remote host closed connection");
                        continue;
                    }
                    if (socket->readBufSize() > maxLineSize)
                    {
                        disconnect(socket, "Socket's buffer size has exceed the limit");
                        continue ;
                    }
                    if (line.empty())
						continue ;
					std::cout << line;
                    exec(_network.getConnBySocket(socket), IRC::Message(line));
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                disconnect(socket, e.what());
                continue;
            }
        }
		pingpongProbe();
    }
}

void Server::writeMessage(User &user, const std::string &command, const std::string &content)
{
	user.sendMessage((IRC::MessageBuilder(_setting.serverName, command) << user.nickname() << content).str());
}

int Server::writeNumber(User &user, const  IRC::Numeric &response)
{
	user.sendMessage(IRC::MessageBuilder(_setting.serverName, response, user.nickname()).str());
	return (-1);
}

void Server::disconnect(tcp::TcpSocket *socket, const std::string &reason) throw()
{
	Connection *conn = _network.getConnBySocket(socket);
	disconnect(*static_cast<User *>(conn), reason);
}

void Server::disconnect(User &user, const std::string &reason, bool notifyUserQuit) throw()
{
	const std::string quitMessage = (IRC::MessageBuilder(user.label(), "QUIT") << reason).str();
	std::stringstream errorReason;

	if (user.joinedChannels())
	{
		_network.resetUserReceipt();
		const Network::ChannelMap &channels = _network.channels();
		Network::ChannelMap::const_iterator i = channels.begin();
		while (i != channels.end())
		{
			Channel *channel = i->second;
			++i;
			if (channel->findMember(&user))
			{
				channel->delMember(&user);
				if (!channel->count())
					_network.remove(channel);
				else
				{
					channel->send(quitMessage, NULL, true);
					channel->markAllMembers();
				}
			}
		}
	}
	errorReason << "Closing Link: " << user.socket()->host();
	if (notifyUserQuit)
		errorReason << " (Client Quit)";
	else
		errorReason << " (" << reason << ')';
	writeError(user.socket(), errorReason.str());
	_network.remove(&user);
	_network.addLostConn(&user);
	std::cout << user.socket()->host() << ' ' << errorReason.str() << std::endl;
}

int Server::exec(Connection *sender, const IRC::Message &msg)
{
    if (!msg.isValid())
		return (-1);
    
    User &user = *static_cast<User*>(sender);
    CmdTypeMap::const_iterator i = _commands.find(user.type());
    if (i == _commands.end())
    {
        std::cout << "Command type not defined!" << std::endl;
        return 1;
    }
	std::cout << "debug: " + msg.command() << std::endl;
    CmdMap::const_iterator j = i->second.find(msg.command());
    if (j == i->second.end())
        return (writeNumber(user, IRC::Error::unknowncommand(msg.command())));
    
    int commandStatus = (this->*(j->second))(user, msg);
    CommandStats &stats = _commandsStats[msg.command()];
	++stats.count;
	stats.byteCount += msg.entry().size();
	return (commandStatus);
}

void Server::writeMotd(User &user)
{
	if (!_setting.motd.size())
	{
		writeNumber(user, IRC::Error::nomotd());
		return ;
	}
	writeNumber(user, IRC::Reply::motdstart(_setting.serverName));
	for (size_t i = 0; i < _setting.motd.size(); ++i)
		writeNumber(user, IRC::Reply::motd(_setting.motd[i]));
	writeNumber(user, IRC::Reply::endofmotd());
}

void Server::writeWelcome(User &user)
{
	writeNumber(user, (user.type() == User::USER ? IRC::Reply::welcome(user.label()) : IRC::Reply::youreservice(user.nickname())));
	writeNumber(user, IRC::Reply::yourhost(_setting.serverName, _version));
	if (user.type() == User::USER)
		writeNumber(user, IRC::Reply::created(_creationDate));
	writeNumber(user, IRC::Reply::myinfo(_setting.serverName, _version, "aiorsw", "IObeiklmnopstv"));
	if (user.type() == User::USER)
		writeMotd(user);
}

void Server::writeError(tcp::TcpSocket *socket, std::string reason)
{
	socket->writeLine((IRC::MessageBuilder(_setting.serverName, "ERROR") << reason).str());
}

void Server::closeLostConnections()
{
	Connection *conn;

	while ((conn = _network.nextLostConn()))
	{
		try {
			conn->socket()->sendBuffer();
		} catch(std::exception &e) {}
		_tcpSrv.disconnect(conn->socket());
		delete conn;
	}
}

void Server::pingpongProbe()
{
	static time_t ptime = 0;
	time_t ctime = ::time(NULL);

	if (ctime - ptime < 5)
		return ;
	ptime = ctime;
	for (Network::ConnectionMap::const_iterator it = _network.connections().begin(); it != _network.connections().end();) {
		Connection *c = it->second;
		it++;
		if (c->pongExpected() && ctime - c->clock() > _setting.pong)
			disconnect(c->socket(), "Ping timeout");
		else if (!c->pongExpected() && ctime - c->clock() > _setting.ping)
		{
			c->sendMessage("PING :" + _setting.serverName);
			c->clock() = ctime;
			c->pongExpected() = true;
		}
	}
}