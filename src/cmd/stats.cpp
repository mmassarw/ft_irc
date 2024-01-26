#include "Server.hpp"
#include "libft.hpp"

// provides information about server statistics based on the parameters:
// m = commands usage statistics
// o = operator information
// u = server uptime

int Server::stats(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() > 1 && !ft::match(m.params()[1], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[1])));
	char c = m.params().size() && m.params()[0].size() == 1 ? m.params()[0][0] : 0;
	if (c == 'm')
		for (CommandsStatsMap::const_iterator i = _commandsStats.begin(); i != _commandsStats.end(); ++i)
			writeNumber(u, IRC::Reply::statscommands(i->first, i->second.count, i->second.byteCount, 0));
	else if (c == 'o')
		for (Config::OperatorMap::const_iterator i = _setting.operators.begin(); i != _setting.operators.end(); ++i)
			writeNumber(u, IRC::Reply::statoline(i->second->host, i->first));
	else if (c == 'u')
		writeNumber(u, IRC::Reply::statuptime(::time(NULL) - _creation));
	else
		return (0);
	writeNumber(u, IRC::Reply::endofstats(m.params()[0]));
	return (0);
}
