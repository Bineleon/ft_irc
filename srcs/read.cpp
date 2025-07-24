#include "../includes/Server.hpp"

void Server::rmClientFromChannels(Client *clientToRm)
{
	std::vector<std::string> channelsToRm;

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel *chan = it->second;
		chan->kickUser(clientToRm);

		if (chan->getUsers().empty())
			channelsToRm.push_back(it->first);
	}

	for (size_t i = 0; i < channelsToRm.size(); ++i)
	{
		delete _channels[channelsToRm[i]];
		_channels.erase(channelsToRm[i]);
	}
}

void Server::closeClient(Client *client)
{
	std::vector<struct pollfd>::iterator it;
	for (it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == client->getFd())
		{
			_pollFds.erase(it);
			break;
		}
	}
	close(client->getFd());

	std::map<int, Client*>::iterator cit = _clients.find(client->getFd());
	std::string nick = cit->second->getNickname();
	if (cit != _clients.end())
	{
		rmClientFromChannels(cit->second);
		delete cit->second;
		_clients.erase(cit);
		_nickClients.erase(nick);
	}
}

void Server::closeClient(struct pollfd pfdClient)
{
	if (_pollFds.empty())
		return;
	std::vector<struct pollfd>::iterator it;
	for (it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == pfdClient.fd)
		{
			close(pfdClient.fd);
			break;
		}
	}
	if (it != _pollFds.end())
		_pollFds.erase(it);
	
	std::map<int, Client*>::iterator cit = _clients.find(pfdClient.fd);
	std::string nick = cit->second->getNickname();
	if (cit != _clients.end())
	{
		rmClientFromChannels(cit->second);
		delete cit->second;
		_clients.erase(cit);
		_nickClients.erase(nick);
	}
}

void Server::readFromSocket(struct pollfd pfdClient)
{
	char buffer[BUFFER_SIZE];
	int bytesRead;
	std::string toParse;

	std::memset(&buffer, 0, sizeof buffer);
	bytesRead = recv(pfdClient.fd, buffer, BUFFER_SIZE, 0);
	std::cout << "buffer = " << buffer << "\n";
	if (bytesRead <= 0)
	{
		if (bytesRead == 0)
		{
			std::cout << "[" << pfdClient.fd << "] Client socket closed connection." << std::endl;
			closeClient(pfdClient);
		}
		else
			perror("recv()");
	}
	else
	{
		_clients[pfdClient.fd]->appendToMsgBuf(buffer);
		std::string clientMsgBuf = _clients[pfdClient.fd]->getmsgBuffer();
		size_t pos;

		while ((pos = clientMsgBuf.find("\r\n")) != std::string::npos)
		{
			toParse = clientMsgBuf.substr(0, pos);
			clientMsgBuf.erase(0, pos + 2);
			std::cout << "RECEIVED: " << clientMsgBuf << std::endl;
            _clients[pfdClient.fd]->setMsgBuffer(clientMsgBuf);
			if (!toParse.empty())
			{
				fullCmd cmd = parseCmd(toParse);
				executeCmd(cmd, _clients[pfdClient.fd]);
			}
		}
	}
}


bool Server::chanIsOnServer(std::string chanName)
{
	if (_channels.find(chanName) == _channels.end())
		return false;
	return true;
}


CMD_TYPE Server::checkCMD(fullCmd cmd)
{
	if (cmd.verb == "PASS")
		return PASS;
	else if (cmd.verb == "NICK")
		return NICK;
	else if (cmd.verb == "USER")
		return USER;
	else if (cmd.verb == "PRIVMSG")
		return PRIVMSG;
	else if (cmd.verb == "JOIN")
		return JOIN;
	else if (cmd.verb == "KICK")
		return KICK;
	else if (cmd.verb == "INVITE")
		return INVITE;
	else if (cmd.verb == "TOPIC")
		return TOPIC;
	else if (cmd.verb == "MODE")
		return MODE;
	else if (cmd.verb == "PING")
		return PING;
	else if (cmd.verb == "CAP")
		return CAP;
	else if (cmd.verb == "PART")
		return PART;
	else if (cmd.verb == "QUIT")
		return QUIT;
	else
		return UNKNOWN;
}


void Server::executeCmd(fullCmd cmd, Client *client)
{
	CMD_TYPE cmdType = checkCMD(cmd);
	printCmd(cmd);

	if (client->getStatus() == PASSWORD_NEEDED && cmdType != PASS && cmdType != CAP && cmdType != JOIN)
	{
		client->sendMessage("ERROR :Password required");
		return ;
	}
	else
	{
		switch (cmdType)
		{
			case PASS:
				passCmd(cmd, client);
				break;
			case NICK:
				nickCmd(cmd, client);
				break;
			case USER:
				userCmd(cmd, client);
				break;
			case PRIVMSG:
				privmsgCmd(cmd, client);
				break;
			case JOIN:
				joinCmd(cmd, client);
				break;
			case KICK:
				kickCmd(cmd, client);
				break;
			case INVITE:
				inviteCmd(cmd, client);
				break;
			case TOPIC:
				topicCmd(cmd, client);
				break;
			case MODE:
				modeCmd(cmd, client);
				break;
			case PING:
				pongCmd(cmd, client);
				break;
			case PART:
				partCmd(cmd, client);
				break;
			case QUIT:
				quitCmd(cmd, client);
				break;
			case CAP:
				break;
			default:
				sendReply(client, ERR_UNKNOWNCOMMAND, cmd.verb, "Unknown command");		
				printCmd(cmd);
				break;
		}
	}

}
