#pragma once

#include <map>
#include <set>
#include "User.hpp"
#include "MemberMode.hpp"
#include "ChannelMode.hpp"

class MaskSet : public std::set<std::string>
{
public:
	MaskSet();
	~MaskSet();

	bool inSet(const User &user) const;
};

class Channel
{
	typedef std::set<std::string>		InvitationSet;
	typedef std::map<User*, MemberMode>	MemberMap;

	enum Type
	{
		/** '#' */
		GLOBAL,
		/** '+' */
		UNMODERATED
	};

	enum MaskSetType
	{
		BAN_SET,
		EXCEPTION_SET,
		INVITATION_SET
	};

    public:
		Channel(const std::string &name = "");
		~Channel();

		const std::string	&name() const;
		Type				type() const;
		const ChannelMode	&mode() const;
		const MemberMap		&members() const;
		const std::string	&topic() const;
		const std::string	&key() const;
		size_t				limit() const;
		const MaskSet		&masks(const MaskSetType type) const;
		void				setMode(const ChannelMode &mode);
		void				setTopic(const std::string &topic);
		void				setKey(const std::string &key);
		void				setLimit(size_t limit);
		size_t				count() const;
		bool				isBanned(const User *user) const;
		bool				isInvited(const User *user) const;
		size_t				nbUserVisible() const;
		void				send(const std::string &msg, Connection *origin = NULL, bool useReceipt = false) const;
		void				addMember(User *user, const MemberMode &mode);
		void				addMask(const MaskSetType type, const std::string &mask);
		bool				canSendToChannel(User *user);
		void				delMember(User *user);
		void				delMask(const MaskSetType type, const std::string &mask);
		MemberMode			*findMember(User *user);
		void				invite(const User *user);
		void				markAllMembers();

	private:
		const std::string	_name;
		const Type			_type;
		ChannelMode			_mode;
		MemberMap			_members;
		std::string			_topic;
		std::string			_key;
		size_t				_limit;
		MaskSet				_masks[3];
		InvitationSet		_invitations;
};