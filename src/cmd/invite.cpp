#include "Server.hpp"

//invites a user to a channel, returns 0 to indicate successful execution.

int Server::invite(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &nick = m.params()[0], chan = m.params()[1];
	User *d;
	if (!nick.isNickname() || !(d = _network.getUserByNickname(nick)))
		return (writeNumber(u, IRC::Error::nosuchnick(nick)));
	Channel *c;
	if (!chan.isChannel() || !(c = _network.getChannelByName(chan)))
		return (writeNumber(u, IRC::Error::nosuchchannel(chan)));
	MemberMode *mm = c->findMember(&u);
	if (!mm)
		return (writeNumber(u, IRC::Error::notonchannel(chan)));
	if (c->findMember(d))
		return (writeNumber(u, IRC::Error::useronchannel(nick, chan)));
	if (c->mode().isSet(ChannelMode::INVITE_ONLY) && !mm->isSet(MemberMode::OPERATOR))
		return (writeNumber(u, IRC::Error::chanoprisneeded(chan)));
	c->invite(d);
	IRC::MessageBuilder r(u.label(), m.command());
	r << nick << chan;
	d->sendMessage(r.str());
	writeNumber(u, IRC::Reply::inviting(chan, nick));
	if (d->userMode().isSet(UserMode::AWAY))
		writeNumber(u, IRC::Reply::away(d->nickname(), d->awayReason()));
	return (0);
}
