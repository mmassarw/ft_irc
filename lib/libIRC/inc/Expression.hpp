#ifndef EXPRESSION_HPP
# define EXPRESSION_HPP
# include "Context.hpp"

namespace IRC
{

class Expression
{
public:
	Expression();
	virtual ~Expression();

	bool isValid() const;

	virtual bool interpret(Context &c) = 0;

protected:
	bool accept();
	bool reject();

private:
	bool _isValid;
};

}

#endif
