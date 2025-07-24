#include "../includes/Channel.hpp"

Channel::Channel(void)
{
}

Channel::Channel(std::string name): _name(name), _hasKey(false), _hasUserLimit(false), _hasTopicRestric(false), _hasTopic(false), _isInviteOnly(false)
{
}

Channel::Channel(std::string name, std::string key): _name(name), _key(key), _hasKey(true), _hasUserLimit(false), _hasTopicRestric(false), _hasTopic(false), _isInviteOnly(false)
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
	_users.clear();
	_operators.clear();
	_invited.clear();
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

std::map<std::string, Client*>&	Channel::getUsers()
{
	return _users;
}

std::map<std::string, Client*>&	Channel::getOperators()
{
	return _operators;
}

std::map<std::string, Client*>&	Channel::getInvited()
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

void	Channel::setHasTopic(bool hasTopic)
{
	_hasTopic = hasTopic;
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

bool	Channel::isInvited(Client *client)
{
	if (_invited.find(client->getNickname()) == _invited.end())
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
	if (isInvited(client))
		_invited.erase(client->getNickname());
}

void Channel::invite(Client *client)
{
	_invited[client->getNickname()] = client;
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

void Channel::modeRPL(Client *client, char mode, bool add, std::string const &param)
{
	std::ostringstream modeMsg;
	
	modeMsg << ":" << client->getMask() << " MODE " << _name << " " << (add ? "+" : "-") << mode;

	if (mode == 'k' || mode == 'o' || mode == 'l')
		modeMsg << " " << param;
	broadcast(modeMsg.str(), NULL);
}

void	Channel::handleModes(Server *serv, Client *client, std::string const &modes, std::vector<std::string> const &modesParams)
{
	std::vector<std::string> params;
	if (modes.empty())
	{
		serv->sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
		return;
	}
	if ((modes[0] != '-' && modes[0] != '+'))
	{
		params.push_back(modes);
		serv->sendReply(client, ERR_UNKNOWNMODE, params, "is unknown mode char to me");
		return;
	}

	bool add;
	size_t idxParams = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		char mode = modes[i];
		if (mode == '+' || mode == '-')
		{
			checkSign(mode, add);
			continue;
		}

		switch (mode)
		{
		case 'i':
			_isInviteOnly = add;
			modeRPL(client, mode, add, "");
			break;
		case 't':
			_hasTopicRestric = add;
			modeRPL(client, mode, add, "");
			break;
		case 'k':
			handleKeyMode(serv, client, add, modesParams, idxParams, mode);
			break;
		case 'o':
			handleOpMode(serv, client, add, modesParams, idxParams, mode);
			break;
		case 'l':
			handleLimitMode(serv, client, add, modesParams, idxParams, mode);
			break;
		default:
			serv->sendReply(client, ERR_UNKNOWNMODE, std::string(1, mode), "is unknown mode char to me");
			break;
		}
	}
}

void	Channel::handleKeyMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode)
{
	if (add)
	{
		if (idx >= params.size() || params[idx].empty())
		{
			serv->sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
			return;
		}
		_key = params[idx++];
		_hasKey = true;
		modeRPL(client, mode, add, _key);
	}
	else
	{
		_key.clear();
		_hasKey = false;
		modeRPL(client, mode, add, "");
	}
}

void	Channel::handleOpMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode)
{
	if (idx >= params.size() || params[idx].empty())
	{
		serv->sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
		return;
	}
	
	std::string nickname = params[idx++];
	if (!isUser(nickname))
	{
		std::vector<std::string> params;
		params.push_back(client->getNickname());
		params.push_back(_name);
		serv->sendReply(client, ERR_USERNOTINCHANNEL, params, "They aren't on that channel");
		return;
	}
	if (add)
		addOperator(_users[nickname]);
	else
		rmOperator(_users[nickname]);
	modeRPL(client, mode, add, nickname);
}

void	Channel::handleLimitMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode)
{
	int limit;

	if (add)
	{
		if (idx >= params.size() || params[idx].empty())
		{
			serv->sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
			return;
		}
		if (!convertToInt(params[idx], limit) || limit <= 0)
		{
			serv->sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
			return;
		}
		_userLimit = limit;
		_hasUserLimit = true;
		modeRPL(client, mode, add, params[idx++]);
	}
	else
	{
		_hasUserLimit =  false;
		modeRPL(client, mode, add, "");
	}
}

void	Channel::broadcast(std::string const &msg, Client *except)
{
	for (std::map<std::string, Client*>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second != except)
			it->second->sendMessage(msg);
	}
}

void Channel::rmClient(const std::string& nickname)
{
	_users.erase(nickname);
	_operators.erase(nickname);
	_invited.erase(nickname);
}
