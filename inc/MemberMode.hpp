#pragma once

#include "Mode.hpp"

class MemberMode : public Mode
{
public:
	enum Flag
	{
		/** 'O' - Channel creator status */
		CREATOR = 1,
		/** 'o' - Channel operator privilege */
		OPERATOR = 1 << 2,
		/** 'v' - Voice privilege */
		VOICE = 1 << 3
	};

	MemberMode(unsigned flags = 0);
	~MemberMode();

	bool canTalk() const;

	static Flag	parse(char c);
};