#include "logError.hpp"

namespace ft
{
    logError::logError(const std::string &err_prefix) :
        std::runtime_error(err_prefix + std::string(": ") + std::strerror(errno)), _code(errno)
    {}

    logError::logError(const int code, const std::string &err_prefix) :
        std::runtime_error(err_prefix + std::string(": ") + std::strerror(code)), _code(code)
    {}

    logError::~logError() throw()
    {}

    int logError::code() const throw()
    {
        return (_code);
    }
}