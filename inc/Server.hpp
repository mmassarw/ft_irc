#pragma once

#include "TcpServer.hpp"
#include "Config.hpp"
#include "IRC.hpp"
#include "User.hpp"
#include "Network.hpp"

struct ServerSetting
{
    ServerSetting() {}
	~ServerSetting() {}

    std::string             	serverName;
    std::string             	serverPass;
	std::string					serverDesc;
    std::string             	tcpPort;
	size_t						historySize;
    int                     	maxConnections;
	size_t                     	maxChannels;
	size_t						maxMasks;
	time_t						ping;
	time_t						pong;
	std::vector<std::string>	motd;
	Config::ServerHostMap		serverHosts;
	Config::OperatorMap     	operators;
};

struct CommandStats
{
	size_t	count;
	size_t 	byteCount;

	CommandStats();
};

class Server
{
    public:
        Server(Config &config, bool _autoInit = true);
        ~Server();

        enum State
        {
            ACTIVE,
            RESTART,
            SHUTDOWN
        };

        State			state() const;
        void            init();
        void            run();

    private:
        Server();

		typedef int (Server::*CommandPointer)(User &, const IRC::Message &);
		typedef std::map<std::string, CommandPointer> CmdMap;
		typedef std::map<unsigned, CmdMap> CmdTypeMap;
		typedef std::map<std::string, CommandStats> CommandsStatsMap;
		typedef IRC::Message::Params Params;

		static const std::string _version;
        static const size_t maxLineSize;
        
        State				_state;
        time_t				_creation;
		std::string			_creationDate;
        tcp::TcpServer		_tcpSrv;
        ServerSetting   	_setting;
        Network				_network;
		CmdTypeMap			_commands;
		CommandsStatsMap	_commandsStats;

		void			disconnect(tcp::TcpSocket *socket, const std::string &reason) throw();
		void			disconnect(User &user, const std::string &reason, bool notifyUserQuit = false) throw();
        void	        writeMessage(User &user, const std::string &command, const std::string &content);
		int 			writeNumber(User &user, const  IRC::Numeric &response);
        int             exec(Connection *sender, const IRC::Message &msg);
		void			writeWelcome(User &user);
		void			writeMotd(User &user);
		void			writeError(tcp::TcpSocket *socket, std::string reason);
		void 			pingpongProbe();
		void 			closeLostConnections();

		int				away(User &sender, const IRC::Message &msg);
		int				die(User &sender, const IRC::Message &msg);
		int				info(User &sender, const IRC::Message &msg);
		int				invite(User &sender, const IRC::Message &msg);
		int				join(User &sender, const IRC::Message &msg);
		int				kick(User &sender, const IRC::Message &msg);
		int				kill(User &sender, const IRC::Message &msg);
		int				list(User &sender, const IRC::Message &msg);
		int				lusers(User &sender, const IRC::Message &msg);
		int				mode(User &sender, const IRC::Message &msg);
		int				motd(User &sender, const IRC::Message &msg);
		int				names(User &sender, const IRC::Message &msg);
		int				nick(User &sender, const IRC::Message &msg);
		int				notice(User &sender, const IRC::Message &msg);
		int				oper(User &sender, const IRC::Message &msg);
		int				part(User &sender, const IRC::Message &msg);
		int				pass(User &sender, const IRC::Message &msg);
		int				ping(User &sender, const IRC::Message &msg);
		int				pong(User &sender, const IRC::Message &msg);
		int				privmsg(User &sender, const IRC::Message &msg);
		int				quit(User &sender, const IRC::Message &msg);
		int 			rehash(User &sender, const IRC::Message &msg);
		int 			restart(User &sender, const IRC::Message &msg);
		int				service(User &sender, const IRC::Message &msg);
		int				servlist(User &sender, const IRC::Message &msg);
		int				squery(User &sender, const IRC::Message &msg);
		int				stats(User &sender, const IRC::Message &msg);
		int				time(User &sender, const IRC::Message &msg);
		int				topic(User &sender, const IRC::Message &msg);
		int				user(User &sender, const IRC::Message &msg);
		int 			userhost(User &sender, const IRC::Message &msg);
		int				version(User &sender, const IRC::Message &msg);
		int				who(User &sender, const IRC::Message &msg);
		int 			whois(User &sender, const IRC::Message &msg);
		int 			whowas(User &sender, const IRC::Message &msg);
};
