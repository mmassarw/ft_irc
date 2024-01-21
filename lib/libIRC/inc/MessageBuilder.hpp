#ifndef MESSAGEBUILDER_HPP
# define MESSAGEBUILDER_HPP
# include <sstream>
# include "Numeric.hpp"

namespace IRC
{

class MessageBuilder
{
public:
	MessageBuilder(const std::string &command);
	MessageBuilder(const std::string &prefix, const std::string &command);
	MessageBuilder(const std::string &prefix, const IRC::Numeric &numeric, const std::string &target);
	~MessageBuilder();

	const std::string &command() const;
	const std::string &prefix() const;
	size_t size() const;
	std::string str() const;

	MessageBuilder &operator<<(const std::string &arg);
	MessageBuilder &operator<<(unsigned arg);

private:
	const std::string	_prefix;
	const std::string	_command;
	std::stringstream	_args;
	std::string			_lastArg;
	bool				_lastArgSet;
	size_t				_size;

	MessageBuilder	&append(const std::string &arg);
};

} /* end of namespace IRC */

#endif