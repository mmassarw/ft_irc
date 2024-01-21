#ifndef PARSER_HPP
# define PARSER_HPP
# include "Context.hpp"

namespace IRC
{

namespace Parser
{
	bool asChannel(Context &c, std::string &s);
	bool asCommand(Context &c, std::string &s);
	bool asHost(Context &c, std::string &s);
	bool asHostname(Context &c, std::string &s);
	bool asNickname(Context &c, std::string &s);
	bool asUser(Context &c, std::string &s);
	bool isNumber(Context &c, size_t min, size_t max);
	bool isXNumber(Context &c, size_t min, size_t max);
	bool isIPv4(Context &c);
	bool isIPv6(Context &c);
	bool isShortname(Context &c);
}

}

#endif // PARSER_HPP