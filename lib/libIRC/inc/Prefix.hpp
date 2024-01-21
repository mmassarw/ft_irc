#ifndef PREFIX_HPP
# define PREFIX_HPP
# include <iostream>
# include "Expression.hpp"

namespace IRC
{

class Prefix : public Expression
{
public:
	Prefix();
	virtual ~Prefix();

	const std::string &servername() const;
	const std::string &nickname() const;
	const std::string &user() const;
	const std::string &host() const;
	bool interpret(Context &c);

private:
	std::string _servername, _nickname, _user, _host;
};

std::ostream &operator<<(std::ostream &o, const Prefix &p);

}

#endif