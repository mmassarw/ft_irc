#pragma once

#include "Mode.hpp"

class UserMode : public Mode
{
public:
	enum Flag
	{
		/** 'a' - User is flagged as away */
		AWAY = 1,
		/** 'i' - Marks a User as invisible */
		INVISIBLE = 1 << 1,
		/** 'w' - User receives wallops */
		WALLOPS = 1 << 2,
		/** 'r' - Restricted User connection */
		RESTRICTED = 1 << 3,
		/** 'o' - Operator flag */
		OPERATOR = 1 << 4,
		/** 's' - Marks a User for receipt of server notices */
		MARK = 1 << 6
	};

	UserMode(unsigned flags = 0);
	~UserMode();

	static Flag	parse(char c);

	std::string	toString() const;

private:
	/** Quick lookup table for lower alphabet */
	static const unsigned short _lowerFlagTable[26];
};