#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(std::string name): _name(name), _hasUserLimit(false)
{
}

Channel::Channel(const Channel& src)
{
    (void) src;
}

Channel& Channel::operator=(const Channel& rhs)
{
    (void) rhs;
    return *this;
}

Channel::~Channel(void)
{
}

void Channel::addOperator(Client *client)
{
    _operators.insert(client);
}

void Channel::addUser(Client *client)
{
    _users.insert(client);
}

JoinStatus Channel::checkJoinStatus(Client *client) const
{
    // TODO
    return J_OK;
}