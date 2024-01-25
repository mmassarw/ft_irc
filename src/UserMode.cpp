#include "UserMode.hpp"

UserMode::UserMode(unsigned flags) : Mode(flags)
{
}

UserMode::~UserMode()
{
}

UserMode::Flag UserMode::parse(char c)
{
	unsigned f;
	f = islower(c) ? _lowerFlagTable[c - 'a'] : (c == 'o') * OPERATOR;
	return (static_cast<Flag>(f));
}

std::string UserMode::toString() const
{
	std::string s;
	if (isSet(INVISIBLE))
		s.push_back('i');
	if (isSet(WALLOPS))
		s.push_back('w');
	if (isSet(RESTRICTED))
		s.push_back('r');
	if (isSet(OPERATOR))
		s.push_back('o');
	return (s);
}

const unsigned short UserMode::_lowerFlagTable[] =
{
	AWAY, 0, 0, 0, 0, 0, 0, 0, INVISIBLE, 0, 0, 0, 0, 0, OPERATOR, 0, 0,
	RESTRICTED, MARK, 0, 0, 0, WALLOPS, 0, 0, 0
};
