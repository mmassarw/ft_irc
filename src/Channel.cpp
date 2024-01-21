#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name)
{
}

Channel::~Channel()
{
}

const std::string &Channel::name() const
{
    return (_name);
}