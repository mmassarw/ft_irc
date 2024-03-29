#include "Server.hpp"

//processes user requests to join channels on an IRC server.

static bool parseNextKeyParam(const std::vector<IRC::Param> &params, size_t &i, IRC::Param &dst)
{
	if (i >= params.size())
		return (0);
	dst = params[i++];
	return (1);
}

int Server::join(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	if (m.params()[0] == "0")
	{
		const Network::ChannelMap &channels = _network.channels();
		Network::ChannelMap::const_iterator i = channels.begin();
		while (i != channels.end())
		{
			Channel *c = i->second;
			++i;
			if (c->findMember(&u))
				part(u, IRC::Message("PART " + c->name()));
		}
		return (0);
	}
	Params		channels(m.params()[0].split());
	Params		keys;
	size_t		keysI = 0;
	IRC::Param	key;
	bool		needMoreParams = 0;
	
	if (m.params().size() > 1)
		keys = m.params()[1].split();
	for (Params::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (!it->isChannel())
			writeNumber(u, IRC::Error::nosuchchannel(*it));
		else if (u.joinedChannels() < _setting.maxChannels)
		{
			bool isBanned = 0, isInvited = 0;
			Channel *c = _network.getChannelByName(*it);
			if (!c)
			{
				c = new Channel(*it);
				_network.add(c);
			}
			else if (c->findMember(&u))
				continue ;
			else
			{
				const ChannelMode cm = c->mode();
				isBanned = c->isBanned(&u);
				isInvited = c->isInvited(&u);
				if (!isInvited)
				{
					if (cm.isSet(ChannelMode::INVITE_ONLY))
					{
						writeNumber(u, IRC::Error::inviteonlychan(*it));
						continue ;
					}
					if (isBanned)
					{
						writeNumber(u, IRC::Error::bannedfromchan(*it));
						continue ;
					}
				}
				if (cm.isSet(ChannelMode::LIMIT) && c->count() == c->limit())
				{
					writeNumber(u, IRC::Error::channelisfull(*it));
					continue ;
				}
				if (cm.isSet(ChannelMode::KEY) && (!parseNextKeyParam(keys, keysI, key) || key != c->key()))
				{
					writeNumber(u, IRC::Error::badchannelkey(*it));
					continue ;
				}
			}
			c->addMember(&u, MemberMode(c->count() || c->type() == Channel::UNMODERATED ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			c->send((IRC::MessageBuilder(u.label(), m.command()) << *it).str());
			if (c->count() == 1)
			{
				if (c->type() == Channel::UNMODERATED)
				{
					c->setMode(ChannelMode(ChannelMode::TOPIC_SETTABLE_BY_CHANOP));
					mode(u, IRC::Message("MODE " + *it));
				}
			}
			else
				topic(u, IRC::Message("TOPIC " + *it));
			names(u, IRC::Message("NAMES " + *it));
		}
		else
			writeNumber(u, IRC::Error::toomanychannels(*it));
	}
	return (needMoreParams ? writeNumber(u, IRC::Error::needmoreparams(m.command())) : 0);
}
