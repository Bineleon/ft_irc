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
		sendReply(client, ERR_NOSUCHCHANNEL, chanName, "No such channel");
		return;
	}
	if (cmd.trailing.empty())
	{
		sendReply(client, ERR_NOTEXTTOSEND, NULL, "No text to send");
		return;
	}
	std::string pvMsg = buildPrivmsg(cmd, client);
	_channels[chanName]->broadcast(pvMsg, client);
}

void Server::handleUserPrivmsg(fullCmd cmd, Client *client)
{
	debug("usermsg");
	std::string const &target = cmd.params[0];
	if (_nickClients.find(target) == _nickClients.end())
	{
		sendReply(client, ERR_NOSUCHNICK, client->getNickname(), "No such nick");
		return;
	}
	if (cmd.trailing.empty())
	{
		sendReply(client, ERR_NOTEXTTOSEND, NULL, "No text to send");
		return;
	}
	std::string pvMsg = buildPrivmsg(cmd, client);
	_nickClients[target]->sendMessage(pvMsg);
	debug("msg sent :" + pvMsg);
}

void Server::privmsgCmd(fullCmd cmd, Client *client)
{
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd))
	{
		sendReply(client, ERR_NORECIPIENT, NULL, "No recipient given");
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
		Channel *channel;
		if (key.empty())
			channel = new Channel(chanName);
		else
			channel = new Channel(chanName, key);
		channel->addOperator(client);
		_channels[chanName] = channel;
	}
	return _channels[chanName];
}

void Server::joinCmd(fullCmd cmd, Client *client)
{
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd))
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
		return;
	}
	
	std::vector<std::string> splitChan = splitCmds(cmd.params[0]);
	bool keys = cmd.params.size() > 1 ? true : false;
	std::vector<std::string> splitKeys;
	if (keys)
		splitKeys = splitCmds(cmd.params[1]);

	for (size_t i = 0; i < splitChan.size(); ++i)
	{
		std::string key = (keys && (i < splitKeys.size())) ? splitKeys[i] : "";

		if (!isValidChanName(splitChan[i]))
		{
			sendReply(client, ERR_NOSUCHCHANNEL, splitChan[i] , "No such channel");
			continue;
		}
		Channel *channel = handleJoinChan(client, key, splitChan[i]);
		JoinStatus status = checkJoinStatus(channel, client, key);
		if (status != J_OK)
		{
			handleJoinErr(client, status, channel);
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
	else if (channel->getHasKey() && (key.empty() || key != channel->getKey()))
		return J_BAD_K;
	else if (channel->getUsers().find(client->getNickname()) != channel->getUsers().end())
		return J_ALRDYIN;
	else if (channel->getIsInviteOnly() && channel->getInvited().find(client->getNickname()) == channel->getInvited().end())
		return J_INVIT_O;
    return J_OK;
}

void Server::handleJoinErr(Client *client, JoinStatus status, Channel *channel)
{
	std::vector<std::string> params;
	switch (status)
	{
		case J_FULL:
			sendReply(client, ERR_CHANNELISFULL, channel->getName() , "Cannot join channel (+l)");
			break;
		case J_BANNED:
			sendReply(client, ERR_BANNEDFROMCHAN, channel->getName() , "Cannot join channel (+b)");
			break;
		case J_BAD_K:
			sendReply(client, ERR_BADCHANNELKEY, channel->getName() , "Cannot join channel (+k)");
			break;
		case J_ALRDYIN:
			params.push_back(client->getNickname());
			params.push_back(channel->getName());
			sendReply(client, ERR_USERONCHANNEL, params, "is already on channel");
			break;
		case J_INVIT_O:
			sendReply(client, ERR_INVITEONLYCHAN, channel->getName() , "Cannot join channel (+i)");
			break;
		default:
			break;
	}
}

void Server::kickCmd(fullCmd cmd, Client *client)
{
	std::vector<std::string> params;
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd) || cmd.params[1].empty())
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
		return;
	}
	if (!isValidChanName(cmd.params[0]) || _channels.find(cmd.params[0]) == _channels.end())
	{
		sendReply(client, ERR_NOSUCHCHANNEL, cmd.params[0] , "No such channel");
		return;
	}
	Channel *channel = _channels[cmd.params[0]];
	if (channel->getOperators().find(client->getNickname()) == channel->getOperators().end())
	{
		sendReply(client, ERR_CHANOPRIVSNEEDED, channel->getName() , "They aren't on that channel");		
		return;
	}
	if (channel->getUsers().find(cmd.params[1]) == channel->getUsers().end())
	{
		params.push_back(client->getNickname());
		params.push_back(channel->getName());
		sendReply(client, ERR_USERNOTINCHANNEL, params , "You're not channel operator");		
		return;
	}
	Client *clientToKick = channel->getUsers().find(cmd.params[1])->second;
	std::string comment = cmd.trailing.empty() ? "Ciao loser!" : cmd.trailing;
	kickRPL(client, clientToKick, channel, comment);
	channel->kickUser(clientToKick);
}

void Server::inviteCmd(fullCmd cmd, Client *client)
{
	std::vector<std::string> params;
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd) || cmd.params[1].empty())
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
		return;
	}
	if (!chanIsOnServer(cmd.params[1]))
	{
		sendReply(client, ERR_NOSUCHCHANNEL, cmd.params[1] , "No such channel");
		return;
	}

	Channel *chanToInviteTo = _channels[cmd.params[1]];
	if (!chanToInviteTo->isOperator(client))
	{
		sendReply(client, ERR_CHANOPRIVSNEEDED, chanToInviteTo->getName() , "You're not channel operator");		
		return;
	}

	Client *toInvite = _nickClients[cmd.params[0]];
	if (chanToInviteTo->isUser(toInvite))
	{
		params.push_back(toInvite->getNickname());
		params.push_back(chanToInviteTo->getName());
		sendReply(client, ERR_USERONCHANNEL, params, "is already on channel");
		return;
	}
	{
 		inviteRPL(client, toInvite, chanToInviteTo);
		sendInvite(client, toInvite, chanToInviteTo);
	}
}

void Server::topicCmd(fullCmd cmd, Client *client)
{
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd))
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
		return;
	}
	if (!chanIsOnServer(cmd.params[0]))
	{
		sendReply(client, ERR_NOSUCHCHANNEL, cmd.params[0] , "No such channel");
		return;
	}
	Channel *targetChannel = _channels[cmd.params[0]];
	if (!targetChannel->isUser(client))
	{
		sendReply(client, ERR_NOTONCHANNEL, targetChannel->getName() , "You're not on that channel");
		return;
	}
	if (!targetChannel->isOperator(client) && targetChannel->getHasTopicRestric())
	{
		sendReply(client, ERR_CHANOPRIVSNEEDED, targetChannel->getName() , "You're not channel operator");		
		return;
	}
	if (cmd.params.size() < 2 && cmd.trailing.empty())
	{
		topicRPL(client, targetChannel);
		return;
	}
	if (cmd.trailing.empty())
		targetChannel->setTopic(cmd.params[1]);
	else
		targetChannel->setTopic(cmd.trailing);
	targetChannel->setHasTopic(true);
	topicRPL(client, targetChannel);
}

void Server::modeCmd(fullCmd cmd, Client *client)
{
	if (!checkAuthenticated(client))
		return;
	if (checkNeedMoreParams(cmd) || cmd.params.size() < 2)
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
		return;
	}
	if (!isValidChanName(cmd.params[0]))
		return;
	if (!chanIsOnServer(cmd.params[0]))
	{
		sendReply(client, ERR_NOSUCHCHANNEL, cmd.params[0] , "No such channel");
		return;
	}
	Channel *targetChannel = _channels[cmd.params[0]];
	if (!targetChannel->isOperator(client))
	{
		sendReply(client, ERR_CHANOPRIVSNEEDED, targetChannel->getName() , "You're not channel operator");		
		return;
	}
	std::string modes = cmd.params[1];
	std::vector<std::string> modesParams(cmd.params.begin() + 2, cmd.params.end());
	targetChannel->handleModes(this, client, modes, modesParams);
}

void	Server::passCmd(fullCmd cmd, Client *client) {
	if (client->getStatus() != PASSWORD_NEEDED) {
		sendReply(client, ERR_ALREADYREGISTERED, NULL , "You may not reregister");		
		return ;
	}

	if (checkNeedMoreParams(cmd)) {
		sendReply(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
		return ;
	}

	if (cmd.params[0] != getPwd()) {
		sendReply(client, ERR_PASSWDMISMATCH, NULL , "Password incorrect");		
		return ;
	}

	client->sendMessage(":localhost NOTICE AUTH :Password accepted");
	client->setStatus(NICKNAME_NEEDED);
}

void	Server::nickCmd(fullCmd cmd, Client *client) {
	if (checkNeedMoreParams(cmd)) {
		sendReply(client, ERR_NONICKNAMEGIVEN, NULL , "No nickname given");		
		return ;
	}

	std::map<std::string, Client*>::iterator it = _nickClients.find(cmd.params[0]);
	if (it != _nickClients.end()) {
		sendReply(client, ERR_NICKNAMEINUSE, cmd.params[0] , "Nickname is already in use");		
		return ;
	}

	if (isValidNickname(cmd.params[0]) == false) {
		sendReply(client, ERR_ERRONEUSNICKNAME, cmd.params[0] , "Erroneus nickname");		
		return ;
	}

	std::string	msg;

	if (client->getStatus() == NICKNAME_NEEDED) {
		msg = "Requesting the new nick \"" + cmd.params[0] + "\".";
		client->sendMessage(msg);
		client->setNickname(cmd.params[0]);
		msg = "Nickname set to " + client->getNickname() + ".";
		client->sendMessage(msg);
		if (client->getUsername().empty())
			client->setStatus(USERNAME_NEEDED);
		else
		{
			client->setStatus(AUTHENTICATED);
			sendWelcome(client);
		}
	}
	else {
		msg = client->getNickname() + " changed his nickname to " + cmd.params[0] + ".";
		client->sendMessage(msg);
	}

	client->setNickname(cmd.params[0]);
	_nickClients[cmd.params[0]] = client;
}

void	Server::userCmd(fullCmd cmd, Client *client)
{
	if (checkNeedMoreParams(cmd) || cmd.params.size() < 3
		|| (cmd.trailing.empty() && cmd.params.size() < 4)) 
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
		return ;
	}
	if (client->getStatus() == AUTHENTICATED)
	{
		sendReply(client, ERR_ALREADYREGISTERED, NULL , "You may not reregister");		
		return;
	}
	client->setUsername(cmd.params[0]);
	if (cmd.trailing.empty())
		client->setRealname(cmd.params[3]);
	else
		client->setRealname(cmd.trailing);

	if (client->getNickname().empty())
		client->setStatus(NICKNAME_NEEDED);
	else
	{
		client->setStatus(AUTHENTICATED);
		sendWelcome(client);
	}
}

void	Server::pongCmd(fullCmd cmd, Client *client)
{
	if (cmd.params.empty() && cmd.trailing.empty())
	{
		sendReply(client, ERR_NOORIGIN, NULL , "No origin specified");
		return;
	}

	std::string target;
	if (cmd.trailing.empty())
		target = cmd.params[0];
	else
		target = cmd.trailing;

	std::string rpl = "PONG: " + target;
	client->sendMessage(rpl);

}

bool	Server::checkAuthenticated(Client *client)
{
	if (client->getStatus() != AUTHENTICATED)
	{
		sendReply(client, ERR_NOTREGISTERED, NULL , "You have not registered");
		return false;
	}
	return true;
}

void Server::partCmd(fullCmd cmd, Client* client)
{
	if (checkNeedMoreParams(cmd))
	{
		sendReply(client, ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
		return;
	}

	std::vector<std::string> channelNames = splitCmds(cmd.params[0]);
	std::string reason = (cmd.trailing.empty()) ? client->getNickname() : cmd.trailing;

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string chanName = channelNames[i];

		if (_channels.find(chanName) == _channels.end())
		{
			sendReply(client, ERR_NOSUCHCHANNEL, chanName, "No such channel");
			continue;
		}

		Channel* chan = _channels[chanName];

		if (!chan->isUser(client->getNickname()))
		{
			sendReply(client, ERR_NOTONCHANNEL, chanName, "You're not on that channel");
			continue;
		}

		std::string partMsg = ":" + client->getMask() + " PART " + chanName;
		if (!reason.empty())
			partMsg += " :" + reason;
		chan->broadcast(partMsg, client);

		chan->rmClient(client->getNickname());
		if (chan->getUsers().empty())
		{
			delete chan;
			_channels.erase(chanName);
		}
	}
}