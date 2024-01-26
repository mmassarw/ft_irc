#include "Server.hpp"
#include "libft.hpp"

//information about a server.

static const char *info[] = {
"ircserv",
"",
"Based on RFC 2810, 2811, 2812, 2813, 7194",
"",
"This program is free software; you can redistribute it and/or",
"modify it under the terms of the GNU General Public License as",
"published by the Free Software Foundation; either version 3",
"or (at your option) any later version.",
"",
"Written in C++ 98 standard",
"",
"Authors:",
"- Mohammad Massarweh",
"- Faraz Khan",
"- Mukhammadusmon Ganiev",
"",
NULL };

int Server::info(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNumber(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], _setting.serverName))
		return (writeNumber(u, IRC::Error::nosuchserver(m.params()[0])));
	for (size_t i = 0; ::info[i]; ++i)
		writeNumber(u, IRC::Reply::info(::info[i]));
	writeNumber(u, IRC::Reply::info(std::string("Compilation date: ") + __DATE__ " at " + __TIME__));
	return writeNumber(u, IRC::Reply::endofinfo());
}
