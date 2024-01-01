#include "SystemError.hpp"

namespace ft
{
    SystemError::SystemError(const std::string &err_prefix) :
        std::runtime_error(err_prefix + std::string(": ") + std::strerror(errno)), _code(errno)
    {}

    SystemError::SystemError(const int code, const std::string &err_prefix) :
        std::runtime_error(err_prefix + std::string(": ") + std::strerror(code)), _code(code)
    {}

    SystemError::~SystemError() throw()
    {}

    int SystemError::code() const throw()
    {
        return (_code);
    }
}