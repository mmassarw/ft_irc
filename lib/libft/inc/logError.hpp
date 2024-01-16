#pragma once

#include <stdexcept>
#include <cerrno>
#include <string>

namespace ft
{
    class logError : public std::runtime_error
    {
        public:
            logError(const std::string &err_prefix);
            logError(const int code, const std::string &err_prefix);
            virtual ~logError() throw();

            int code() const throw();

        private:
            const int _code;
    };
}