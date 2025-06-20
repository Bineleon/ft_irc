#include "../includes/Server.hpp"

std::vector<std::string> splitCmds(std::string const &param)
{
	std::vector<std::string> splitParams;
	std::istringstream s(param);
	std::string word;

	while (getline(s, word, ','))
		splitParams.push_back(word);
	return splitParams;
}

std::string Server::buildPrivmsg(fullCmd cmd, Client *client)
{
	std::ostringstream oss;
	oss << ":" << client->getMask() << " " << "PRIVMSG " << cmd.params[0] << " :" << cmd.trailing;
	return oss.str();
}

void Server::handleChanPrivmsg(fullCmd cmd, Client *client)
{
	std::string const &chanName = cmd.params[0];
	if (_channels.find(chanName) == _channels.end())
	{
		sendError(*client, ERR_NOSUCHCHANNEL);
		return;
	}
	if (cmd.trailing.empty())
	{
		sendError(*client, ERR_NOTEXTTOSEND);
		return;
	}
	std::string pvMsg = buildPrivmsg(cmd, client);
	_channels[chanName]->broadcast(pvMsg, client);
}

void Server::handleUserPrivmsg(fullCmd cmd, Client *client)
{
	std::string const &target = cmd.params[0];
	if (_nickClients.find(target) == _nickClients.end())
	{
		sendError(*client, ERR_NOSUCHNICK);
		return;
	}
	if (cmd.trailing.empty())
	{
		sendError(*client, ERR_NOTEXTTOSEND);
		return;
	}
	std::string pvMsg = buildPrivmsg(cmd, client);
	_nickClients[target]->sendMessage(pvMsg);
}

void Server::privmsgCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd))
	{
		sendError(*client, ERR_NORECIPIENT);
		return;
	}
	if (isValidChanName(cmd.params[0]))
		handleChanPrivmsg(cmd, client);
	else
		handleUserPrivmsg(cmd, client);

}

Channel* Server::handleJoinChan(Client *client, std::string const &key, std::string chanName)
{
	if (_channels.find(chanName) == _channels.end())
	{
		Channel *channel = new Channel(chanName, key);
		channel->addOperator(client);
		_channels[chanName] = channel;
	}
	return _channels[chanName];
}

void Server::joinCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd))
	{
		sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	std::vector<std::string> splitChan = splitCmds(cmd.params[0]);
	bool keys = cmd.params.size() > 1 ? true : false;

	std::vector<std::string> splitKeys = splitCmds(cmd.params[1]);
	
	// Channel *channel = NULL;
	for (size_t i = 0; i < splitChan.size(); ++i)
	{
		std::string key = (keys && (i < splitKeys.size())) ? splitKeys[i] : "";

		if (!isValidChanName(splitChan[i]))
		{
			sendError(*client, ERR_NOSUCHCHANNEL);
			continue;
		}
		Channel *channel = handleJoinChan(client, key, splitChan[i]);
		JoinStatus status = checkJoinStatus(channel, client, key);
		if (status != J_OK)
		{
			handleJoinErr(client, status);
			continue;
		}
		channel->addUser(client);
		joinRPLs(client, channel);
	}
}

JoinStatus Server::checkJoinStatus(Channel *channel, Client *client, std::string const &key) const
{
	if (channel->getHasUserLimit() && (channel->getUsers().size() >= channel->getUserLimit()))
		return J_FULL;
	else if (channel->getBanned().find(client) != channel->getBanned().end())
		return J_BANNED;
	else if (channel->getHasKey() && (key.empty() || key != channel->getKey()))
		return J_BAD_K;
	else if (channel->getUsers().find(client->getNickname()) != channel->getUsers().end())
		return J_ALRDYIN;
	else if (channel->getIsInviteOnly() && channel->getInvited().find(client) == channel->getInvited().end())
		return J_INVIT_O;
    return J_OK;
}

void Server::handleJoinErr(Client *client, JoinStatus status)
{
	switch (status)
	{
		case J_FULL:
			
			break;
		case J_BANNED:
			sendError(*client, ERR_BANNEDFROMCHAN);
			break;
		case J_BAD_K:
			sendError(*client, ERR_BADCHANNELKEY);
			break;
		case J_ALRDYIN:
			sendError(*client, ERR_USERONCHANNEL);
			break;
		case J_INVIT_O:
			sendError(*client, ERR_INVITEONLYCHAN);
			break;
		default:
			break;
	}
}

void Server::kickCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd) || cmd.params[1].empty())
	{
		sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	if (!isValidChanName(cmd.params[0]) || _channels.find(cmd.params[0]) == _channels.end())
	{
		sendError(*client, ERR_NOSUCHCHANNEL);
		return;
	}
	Channel *channel = _channels[cmd.params[0]];
	if (channel->getOperators().find(client->getNickname()) == channel->getOperators().end())
	{
		sendError(*client, ERR_CHANOPRIVSNEEDED);
		return;
	}
	if (channel->getUsers().find(cmd.params[1]) == channel->getUsers().end())
	{
		sendError(*client, ERR_USERNOTINCHANNEL);
		return;
	}
	Client *clientToKick = channel->getUsers().find(cmd.params[1])->second;
	channel->kickUser(clientToKick);
	// add RPL
}

void Server::inviteCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd) || cmd.params[1].empty())
	{
		sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	if (!chanIsOnServer(cmd.params[1]))
	{
		sendError(*client, ERR_NOSUCHCHANNEL);
		return;
	}

	Channel *chanToInviteTo = _channels[cmd.params[1]];
	if (!chanToInviteTo->isOperator(client))
	{
		sendError(*client, ERR_CHANOPRIVSNEEDED);
		return;
	}

	Client *toInvite = _nickClients[cmd.params[0]];
	if (chanToInviteTo->isUser(toInvite))
	{
		sendError(*client, ERR_USERONCHANNEL);
		return;
	}
	if (chanToInviteTo->invite(toInvite))
	{
		// sendReply(client, ) // TODO 

	}
	
}

void Server::topicCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd))
	{
		sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	if (!chanIsOnServer(cmd.params[0]))
	{
		sendError(*client, ERR_NOSUCHCHANNEL);
		return;
	}
	Channel *targetChannel = _channels[cmd.params[0]];
	if (!targetChannel->isUser(client))
	{
		sendError(*client, ERR_NOTONCHANNEL);
		return;
	}
	if (!targetChannel->isOperator(client) && targetChannel->getHasTopicRestric())
	{
		sendError(*client, ERR_CHANOPRIVSNEEDED);
		return;
	}
	if (cmd.params.size() < 2)
	{
		// sendRplTopic
		return;
	}
	targetChannel->setTopic(cmd.params[1]);
	// sendPlyTopic

}

void Server::modeCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd) || cmd.params.size() < 2)
	{
		sendError(*client, ERR_NEEDMOREPARAMS);
		return;
	}
	if (!chanIsOnServer(cmd.params[0]))
	{
		sendError(*client, ERR_NOSUCHCHANNEL);
		return;
	}
	Channel *targetChannel = _channels[cmd.params[0]];
	if (!targetChannel->isOperator(client))
	{
		sendError(*client, ERR_CHANOPRIVSNEEDED);
		return;
	}

	std::string modes = cmd.params[1];
	std::vector<std::string> modesParams(cmd.params.begin() + 2, cmd.params.end());

	targetChannel->handleModes(this, client, modes, modesParams);
}
