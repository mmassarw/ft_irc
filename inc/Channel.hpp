#pragma once

#include <iostream>

class Channel
{
    public:
		Channel(const std::string &name = "");
		~Channel();

		const std::string	&name() const;

	private:
		const std::string	_name;
};