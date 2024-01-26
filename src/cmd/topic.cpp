#include "Server.hpp"

//view and set the topic of a channel.

int Server::topic(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNumber(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &target = m.params()[0];
	Channel *c;
	if (!target.isChannel() || !(c = _network.getChannelByName(target)))
		return (writeNumber(u, IRC::Error::nosuchchannel(target)));
	if (c->type() == Channel::UNMODERATED)
		return (writeNumber(u, IRC::Error::nochanmodes(target)));
	const MemberMode *mm = c->findMember(&u);
	if (!mm)
		return (writeNumber(u, (c->mode().isSet(ChannelMode::SECRET) ? IRC::Error::nosuchchannel(c->name()) : IRC::Error::notonchannel(target))));
	if (m.params().size() < 2)
		return (writeNumber(u, (c->topic().empty() ? IRC::Reply::notopic(target) : IRC::Reply::topic(target, c->topic()))));
	if (c->mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) && !mm->isSet(MemberMode::OPERATOR))
		return (writeNumber(u, IRC::Error::chanoprisneeded(target)));
	c->setTopic(m.params()[1]);
	c->send((IRC::MessageBuilder(u.label(), m.command()) << target << c->topic()).str());
	return (0);
}
