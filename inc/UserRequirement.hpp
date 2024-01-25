#pragma once

#include "Mode.hpp"

class UserRequirement : public Mode
{
public:
	enum Flag
	{
		PASS = 1,
		NICK = 1 << 1,
		USER = 1 << 2,
		ALL_EXCEPT_PASS = NICK | USER,
		ALL = PASS | ALL_EXCEPT_PASS
	};

	UserRequirement(unsigned flags = 0);
	virtual ~UserRequirement();
};