#include "MemberMode.hpp"

MemberMode::MemberMode(unsigned flags) : Mode(flags)
{}

MemberMode::~MemberMode()
{}

bool MemberMode::canTalk() const
{
	return (isSet(OPERATOR | VOICE));
}

MemberMode::Flag MemberMode::parse(char c)
{
	if (c == 'O')
		return (CREATOR);
	if (c == 'o')
		return (OPERATOR);
	return (static_cast<Flag>((c == 'v') * VOICE));
}