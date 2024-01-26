#include "ChannelMode.hpp"

ChannelMode::ChannelMode(unsigned flags) : Mode(flags)
{}

ChannelMode::~ChannelMode()
{}

ChannelMode::Flag ChannelMode::parse(char c)
{
	unsigned f;
	f = islower(c) ? _lowerFlagTable[c - 'a'] : (c == 'I') * INVITATION_MASK;
	return (static_cast<Flag>(f));
}

std::string ChannelMode::toString() const
{
	std::string s;
	if (isSet(INVITE_ONLY))
		s.push_back('i');
	if (isSet(NO_OUTSIDE_MSG))
		s.push_back('n');
	if (isSet(PRIVATE))
		s.push_back('p');
	if (isSet(SECRET))
		s.push_back('s');
	if (isSet(TOPIC_SETTABLE_BY_CHANOP))
		s.push_back('t');
	if (isSet(KEY))
		s.push_back('k');
	if (isSet(LIMIT))
		s.push_back('l');
	return (s);
}

const unsigned short ChannelMode::_lowerFlagTable[] =
{
	0, BAN_MASK, 0, 0, EXCEPTION_MASK, 0, 0, 0, INVITE_ONLY, 0, KEY,
	LIMIT, MODERATED, NO_OUTSIDE_MSG, 0, PRIVATE, 0, 0, SECRET,
	TOPIC_SETTABLE_BY_CHANOP, 0, 0, 0, 0, 0, 0
};