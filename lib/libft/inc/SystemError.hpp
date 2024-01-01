#ifndef SYSTEMERROR_HPP
# define SYSTEMERROR_HPP

#include <stdexcept>
#include <cerrno>
#include <string>

namespace ft
{
    class SystemError : public std::runtime_error
    {
        public:
            SystemError(const std::string &err_prefix);
            SystemError(const int code, const std::string &err_prefix);
            virtual ~SystemError() throw();

            int code() const throw();

        private:
            const int _code;
    };
}

#endif