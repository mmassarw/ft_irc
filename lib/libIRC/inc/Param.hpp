#ifndef PARAM_HPP
# define PARAM_HPP
# include <vector>
# include "Context.hpp"

namespace IRC
{

class Param : public std::string
{
public:
	Param();
	Param(const std::string &value);
	virtual ~Param();

	bool isChannel() const;
	bool isMask() const;
	bool isNickname() const;
	bool isKey() const;

	std::string mask() const;
	std::vector<Param> split(size_t max = -1, char delimiter = ',') const;

private:
	bool check(bool (*parsingFunc)(Context &, std::string &)) const;
};
}

#endif