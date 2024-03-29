#include <fstream>
#include "libft.hpp"

#define TODATE_BUFSIZE	256

// std::string goes brrr
static bool match_internal(const char *mask, const char *str)
{
	const char m = *mask;
	const char c = *str;
	if (!m)
		return (!c);
	if (m == '\\') // escape character (only if next is a special character)
	{
		const char m1 = mask[1];
		if (m1 == '*' || m1 == '?')
			return (m1 == c ? match_internal(mask + 2, str + 1) : 0);
	}
	if (m == '*') // wildcard matching
	{
		if (match_internal(mask + 1, str))
			return (1);
		if (!c)
			return (0);
	}
	else
	{
		if (m == '?') // any character matching
		{
			if (!c)
				return (0);
		}
		else if (m != c) // else character must match
			return (0);
		++mask;
	}
	return (match_internal(mask, str + 1));
}

namespace ft
{

void fileToData(const std::string &filename, std::vector<std::string> &data, size_t sizeLine)
{
	std::ifstream f(filename.c_str(), std::ios_base::in);
	std::string line;

	if (!f.is_open())
		throw ft::logError("std::ifstream");
	data.clear();
	while (std::getline(f, line))
	{
		if (line.size() > sizeLine)
			line.resize(sizeLine);
		data.push_back(line);
	}
}

bool match(const std::string &mask, const std::string &str) throw()
{
	return (match_internal(mask.c_str(), str.c_str()));
}

std::string toDate(const time_t t, const char *format) throw()
{
	char buf[TODATE_BUFSIZE];

	strftime(buf, sizeof(buf), format, localtime(&t));
	return (buf);
}

void toUpper(std::string &s) throw()
{
	for (size_t i = 0; i < s.size(); ++i)
		s[i] = toupper(s[i]);
}

}