#ifndef CONTEXT_HPP
# define CONTEXT_HPP
# include <string>

namespace IRC
{

class Context
{
public:

	Context(const std::string &data);
	virtual ~Context();

	size_t distance() const;
	void resetDistance();

	std::string extract() const;
	
	bool isNospcrlfcl() const;
	bool isSpace() const;
	bool isSpecial() const;

	char operator*() const;
	Context &operator++();
	Context operator++(int);
	Context &operator=(const Context &c);

private:
	typedef std::basic_string<char>::const_iterator	const_iterator;

	const std::string	&_data;
	const_iterator		_start, _i;
	size_t				_distance;
};

}

#endif //CONTEXT_HPP