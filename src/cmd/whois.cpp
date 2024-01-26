#include "Server.hpp"
#include "libft.hpp"

//detailed information about each specified user.
int Server::whois(User &sender, const IRC::Message &m)
{
	if (!sender.isRegistered())
		return writeNumber(sender, IRC::Error::notregistered());
	if (m.params().size() < 1)
		return writeNumber(sender, IRC::Error::nonicknamegiven());
	Params masks;
	if (m.params().size() == 1)
		masks = Params(m.params()[0].split(5));
	else
	{
		if (!ft::match(m.params()[0].mask(), _setting.serverName))
			return writeNumber(sender, IRC::Error::nosuchserver(m.params()[0]));
		masks = Params(m.params()[1].split());
	}
	for (Params::const_iterator mask = masks.begin(); mask != masks.end(); ++mask)
	{
		bool found = false;
		for (Network::UserMap::const_iterator uit = _network.users().begin(); uit != _network.users().end(); ++uit)
		{
			User *u = uit->second;
			if (ft::match(mask->mask(), u->nickname()))
			{
				if (!found)
					found = true;
				writeNumber(sender, IRC::Reply::whoisuser(u->nickname(), u->username(), u->socket()->host(), u->realname()));
				writeNumber(sender, IRC::Reply::whoisserver(u->nickname(), _setting.serverName, _setting.serverDesc));
				for (Network::ChannelMap::const_iterator cit = _network.channels().begin(); cit != _network.channels().end(); ++cit)
				{
					Channel *c = cit->second;
					MemberMode *m = c->findMember(u);
					std::string status;
					if (!m)
						continue ;
					if (m->isSet(m->OPERATOR))
						status = "@";
					else if (m->isSet(m->VOICE))
						status = "+";
					writeNumber(sender, IRC::Reply::whoischannels(u->nickname(), status, c->name()));
				}
				if (u->userMode().isSet(UserMode::OPERATOR))
					writeNumber(sender, IRC::Reply::whoisoperator(u->nickname()));
				writeNumber(sender, IRC::Reply::whoisidle(u->nickname(), ::time(NULL) - u->idle()));
			}
		}
		if (!found)
			writeNumber(sender, IRC::Error::nosuchnick(mask->mask()));
	}
	return (writeNumber(sender, IRC::Reply::endofwhois(sender.nickname())));
}
