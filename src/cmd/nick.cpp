#include "Server.hpp"

//allowing a user to change their nickname.

int Server::nick(User &u, const IRC::Message &m)
{
	if (u.type() == User::SERVICE)
		return (writeNumber(u, IRC::Error::alreadyregistred()));
	if (!m.params().size())
		return (writeNumber(u, IRC::Error::nonicknamegiven()));
	if (!u.requirements().isSet(UserRequirement::NICK) && u.userMode().isSet(UserMode::RESTRICTED))
		return (writeNumber(u, IRC::Error::restricted()));
	const IRC::Param &nick(m.params()[0]);
	if (!nick.isNickname())
		return (writeNumber(u, IRC::Error::erroneusnickname(nick)));
	if (_network.getUserByNickname(nick) || _network.isFnick(nick))
		return (writeNumber(u, IRC::Error::nicknameinuse(nick)));
	_network.remove(&u);
	std::string oldlabel = u.label();
	u.setNickname(nick);
	_network.add(&u);
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
		{
			writeWelcome(u);
			_network.addNickToHistory(u);
		}
		else if (u.requirements().flags() == UserRequirement::PASS)
		{
			writeNumber(u, IRC::Error::passwdmissmatch());
			disconnect(u, "Bad Password");
		}
	}
	else
	{
		std::string msg((IRC::MessageBuilder(oldlabel, m.command()) << u.nickname()).str());
		const Network::ChannelMap &channels = _network.channels();
		Network::ChannelMap::const_iterator c = channels.begin();
		_network.resetUserReceipt();
		while (c != channels.end())
		{
			Channel *chan = c->second;
			if (chan->findMember(&u))
			{
				chan->send(msg);
				chan->markAllMembers();
			}
			++c;
		}
		if (!u.joinedChannels())
			u.sendMessage(msg);
		_network.addNickToHistory(u);
	}
	return (0);
}
