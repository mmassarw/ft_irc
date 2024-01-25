#pragma once

#include "TcpSocket.hpp"

class Mode
{
	public:
		Mode(unsigned mask = 0);
		virtual ~Mode();

		bool areSet(unsigned mask) const;

		unsigned flags() const;

		bool isSet(unsigned mask) const;

		void set(unsigned mask);

		void unset(unsigned mask);

	private:
		unsigned _flags;
};