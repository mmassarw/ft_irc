#include <cstring>
#include "Context.hpp"

namespace IRC
{

Context::Context(const std::string &data) :
_data(data), _start(data.begin()), _i(_start), _distance(0)
{}

Context::~Context()
{}

size_t Context::distance() const
{
	return (_distance);
}

void Context::resetDistance()
{
	_start = _i;
	_distance = 0;
}

std::string Context::extract() const
{
	return (std::string(_start, _i));
}

bool Context::isNospcrlfcl() const {
    char c = **this;
    bool isNotSpace = !isSpace();
    bool isNotColon = c != ':';
    bool isNotNull = c != '\0';

    return isNotNull && isNotSpace && isNotColon;
}

bool Context::isSpace() const {
    char c = **this;
    return c == ' ' || c == '\r' || c == '\n';
}

bool Context::isSpecial() const {
    char c = **this;
    const std::string specialChars = "[]\\`_^{|}";
    return specialChars.find(c) != std::string::npos;
}


char Context::operator*() const
{
	return (_i == _data.end() ? '\0' : *_i);
}

Context &Context::operator++()
{
	if (_i != _data.end())
	{
		++_i;
		++_distance;
	}
	return (*this);
}

Context Context::operator++(int)
{
	Context copy(*this);

	++*this;
	return (copy);
}

Context &Context::operator=(const Context &rhs)
{
	if (&_data == &rhs._data)
	{
		_start = rhs._start;
		_i = rhs._i;
		_distance = rhs._distance;
	}
	return (*this);
}

}
