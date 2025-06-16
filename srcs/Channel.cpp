#include "Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(std::string name): _name(name), _hasUserLimit(false), _hasTopicRestric(false)
{
}

Channel::Channel(std::string name, std::string key): _name(name), _key(key), _hasUserLimit(false), _hasTopicRestric(false)
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
	_users[client->getNickname()] = client;
}

std::string			const &Channel::getName() const
{
	return _name;
}

std::string			const &Channel::getTopic() const
{
	return _topic;
}

std::string			const &Channel::getKey() const
{
	return _key;
}

std::map<std::string, Client*>	const &Channel::getUsers() const
{
	return _users;
}

std::set<Client*>	const &Channel::getOperators() const
{
	return _operators;
}

std::set<Client*>	const &Channel::getBanned() const
{
	return _banned;
}

std::set<Client*>	const &Channel::getInvited() const
{
	return _invited;
}

size_t				const &Channel::getUserLimit() const
{
	return _userLimit;
}

bool				const &Channel::getHasUserLimit() const
{
	return _hasUserLimit;
}

bool				const &Channel::getHasKey() const
{
	return _hasKey;
}

bool	const &Channel::getIsInviteOnly() const
{
	return _isInviteOnly;
}

bool				const &Channel::getHasTopicRestric() const
{
	return _hasTopicRestric;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void	Channel::setUserLimit(size_t userLimit)
{
	_userLimit = userLimit;
}

void	Channel::setTopicRestric(bool isRestricted)
{
	_hasTopicRestric = isRestricted;
}

void	Channel::setKey(std::string key)
{
	_key = key;
}

void	Channel::setHasKey(bool hasKey)
{
	_hasKey = hasKey;
}

void	Channel::setInviteOnly(bool isInviteOnly)
{
	_isInviteOnly = isInviteOnly;
}

bool	Channel::isUser(Client *client)
{
	std::map<std::string, Client*>::iterator it = _users.find(client->getNickname());
	if (it == _users.end())
		return false;
	return true;
}

bool	Channel::isOperator(Client *client)
{
	if (_operators.find(client) == _operators.end())
		return false;
	return true;
}

void	Channel::kickUser(Client *client)
{
	std::map<std::string, Client*>::iterator it = _users.find(client->getNickname());
	if (it == _users.end())
		return;
	_users.erase(it);
}

bool Channel::invite(Client *client)
{
	return _invited.insert(client).second;
}


// . i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// · l: Set/remove the user limit to channel

void	Channel::handleModes(Server *serv, Client *client, std::string const &modes, std::vector<std::string> const &modesParams)
{
	if (modes.empty() || (modes[0] != '-' && modes[0] != '+'))
	{
		serv->sendError(*client, ERR_UNKNOWNMODE);
		return;
	}

	bool add;
	size_t idxParams = 0;


	for (size_t i = 0; i < modes.size(); ++i)
	{
		char mode = modes[i];
		if (mode == '+')
			add = true;
		else if (mode == '-')
			add = false;
		
		switch (mode)
		{
		case 'i':
			_isInviteOnly = add;
			break;
		case 't':
			_hasTopicRestric = add;
			break;
		case 'k':
			/* code */
			break;
		case 'o':
			/* code */
			break;
		case 'l':
			/* code */
			break;
		default:
			serv->sendError(*client, ERR_UNKNOWNMODE);
			break;
		}
	}
}


// JoinStatus Channel::checkJoinStatus(Client *client, std::string const &key) const
// {
// 	if (_users.size() == _userLimit)
// 		return J_FULL;
// 	else if (_banned.find(client) != _banned.end())
// 		return J_BANNED;
// 	else if (_hasKey && (key.empty() || key != _key))
// 		return J_BAD_K;
// 	else if (_users.find(client) != _users.end())
// 		return J_ALRDYIN;
// 	else if (_isInviteOnly && _invited.find(client) == _invited.end())
// 		return J_INVIT_O;
//     return J_OK;
// }

// void Channel::handleJoinErr(Client *client, JoinStatus status) const
// {
// 	switch (status)
// 	{
// 		case J_FULL:
			
// 			break;
// 		case J_BANNED:
// 			// error ERR_BANNEDFROMCHAN
// 			break;
// 		case J_BAD_K:
// 			// error ERR_BADCHANNELKEY
// 			break;
// 		case J_ALRDYIN:
// 			// error ERR_USERONCHANNEL
// 			break;
// 		case J_INVIT_O:
// 			// error ERR_INVITEONLYCHAN
// 			break;
// 		default:
// 			break;
// 	}
// }
