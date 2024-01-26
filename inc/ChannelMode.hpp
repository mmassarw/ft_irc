#pragma once

#include "Mode.hpp"

class ChannelMode : public Mode
{
public:
	enum Flag
	{
		/** 'i' - Invite-only Channel flag */
		INVITE_ONLY = 1 << 1,
		/** 'm' - Moderated Channel flag */
		MODERATED = 1 << 2,
		/** 'n' - No IRC::Messages to Channel from User on the outside */
		NO_OUTSIDE_MSG = 1 << 3,
		/** 'p' - Private channel flag */
		PRIVATE = 1 << 5,
		/** 's' - Secret channel flag */
		SECRET = 1 << 6,
		/** 't' - Topic settable by Channel operator only flag */
		TOPIC_SETTABLE_BY_CHANOP = 1 << 8,
		/** 'k' - Channel key */
		KEY = 1 << 9,
		/** 'l' - User limit */
		LIMIT = 1 << 10,
		/** 'b' - Ban mask */
		BAN_MASK = 1 << 11,
		/** 'e' - Exception mask */
		EXCEPTION_MASK = 1 << 12,
		/** 'I' - Invitation mask */
		INVITATION_MASK = 1 << 13
	};

	ChannelMode(unsigned flags = 0);
	~ChannelMode();

	static Flag	parse(char c);

	std::string	toString() const;

private:
	/** Quick lookup table for lower alphabet */
	static const unsigned short _lowerFlagTable[26];
};