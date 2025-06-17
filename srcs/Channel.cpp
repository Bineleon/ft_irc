#include "../includes/Channel.hpp"

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
    _operators[client->getNickname()] = client;
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

std::map<std::string, Client*>	const &Channel::getOperators() const
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
	if (_users.find(client->getNickname()) == _users.end())
		return false;
	return true;
}

bool	Channel::isUser(std::string nickname)
{
	if (_users.find(nickname) == _users.end())
		return false;
	return true;
}

bool	Channel::isOperator(Client *client)
{
	if (_operators.find(client->getNickname()) == _operators.end())
		return false;
	return true;
}

void	Channel::rmOperator(Client *client)
{
	if (isOperator(client))
		_operators.erase(client->getNickname());
}

void	Channel::kickUser(Client *client)
{
	rmOperator(client);
	if (isUser(client))
		_users.erase(client->getNickname());
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

void checkSign(char mode, bool &add)
{
	if (mode == '+')
		add = true;
	else if (mode == '-')
		add = false;
}

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

		checkSign(mode, add);

		switch (mode)
		{
		case 'i':
			_isInviteOnly = add;
			break;
		case 't':
			_hasTopicRestric = add;
			break;
		case 'k':
			handleKeyMode(serv, client, add, modesParams, idxParams);
			break;
		case 'o':
			handleOpMode(serv, client, add, modesParams, idxParams);
			break;
		case 'l':
			handleLimitMode(serv, client, add, modesParams, idxParams);
			break;
		default:
			serv->sendError(*client, ERR_UNKNOWNMODE);
			break;
		}
	}
}

void	Channel::handleKeyMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx)
{
	if (add)
	{
		if (idx >= params.size() || params[idx].empty())
		{
			serv->sendError(*client, ERR_NEEDMOREPARAMS);
			return;
		}
		_key = params[idx++];
		_hasKey = true;
	}
	else
	{
		_key.clear();
		_hasKey = false;
	}
}

void	Channel::handleOpMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx)
{
	if (idx >= params.size() || params[idx].empty())
	{
		serv->sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	
	std::string nickname = params[idx++];
	if (!isUser(nickname))
	{
		serv->sendError(*client, ERR_USERNOTINCHANNEL);
		return;
	}
	if (add)
		addOperator(_users[nickname]);
	else
		rmOperator(_users[nickname]);
}

void	Channel::handleLimitMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx)
{
	int limit;

	if (add)
	{
		if (idx >= params.size() || params[idx].empty())
		{
			serv->sendError(*client, ERR_NEEDMOREPARAMS);
			return;
		}
		if (!convertToInt(params[idx++], limit) || limit <= 0)
		{
			serv->sendError(*client, ERR_NEEDMOREPARAMS);
			return;
		}
		_userLimit = limit;
		_hasUserLimit = true;
	}
	else
		_hasUserLimit =  false;
}
