#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(std::string name): _name(name), _hasUserLimit(false)
{
}

Channel::Channel(std::string name, std::string key): _name(name), _key(key),_hasUserLimit(false)
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
	if (! _users.insert(client).second)
	{
		// error already in 

	}
}

JoinStatus Channel::checkJoinStatus(Client *client, std::string const &key) const
{
	if (_users.size() == _userLimit)
		return J_FULL;
	else if (_banned.find(client) != _banned.end())
		return J_BANNED;
	else if (_hasKey && (key.empty() || key != _key))
		return J_BAD_K;
	else if (_users.find(client) != _users.end())
		return J_ALRDYIN;
	else if (_isInviteOnly && _invited.find(client) == _invited.end())
		return J_INVIT_O;
    return J_OK;
}

void Channel::handleJoinErr(Client *client, JoinStatus status) const
{
	switch (status)
	{
		case J_FULL:
			// error ERR_CHANNELISFULL
			break;
		case J_BANNED:
			// error ERR_BANNEDFROMCHAN
			break;
		case J_BAD_K:
			// error ERR_BADCHANNELKEY
			break;
		case J_ALRDYIN:
			// error ERR_USERONCHANNEL
			break;
		case J_INVIT_O:
			// error ERR_INVITEONLYCHAN
			break;
		default:
			break;
	}
}
