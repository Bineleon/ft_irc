#include "../includes/Server.hpp"

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
	if (cit != _clients.end())
	{
		delete cit->second;
		_clients.erase(cit);
	}
}

void Server::readFromSocket(struct pollfd pfdClient)
{
	char buffer[BUFFER_SIZE];
	int bytesRead;
	std::string toParse;

	std::memset(&buffer, 0, sizeof buffer);
	bytesRead = recv(pfdClient.fd, buffer, BUFFER_SIZE, 0);
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
			std::cout << "Msg from fd [" << pfdClient.fd << "]: " << toParse << std::endl;
            _clients[pfdClient.fd]->setMsgBuffer(clientMsgBuf);
		}
		if (!toParse.empty())
		{
			fullCmd cmd = parseCmd(toParse);
			// printCmd(cmd);
			executeCmd(cmd, _clients[pfdClient.fd]);
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
	if (cmd.verb == "/PRIVMSG")
		return PRIVMSG;
	else if (cmd.verb == "/JOIN")
		return JOIN;
	else if (cmd.verb == "/KICK")
		return KICK;
	else if (cmd.verb == "/INVITE")
		return INVITE;
	else if (cmd.verb == "/MODE")
		return MODE;
	else
		return UNKNOWN;
}


void Server::executeCmd(fullCmd cmd, Client *client)
{

	if (client->getStatus() != AUTHENTICATED)
	{
		// authenticate
	}
	else
	{
		CMD_TYPE cmdType = checkCMD(cmd);
		switch (cmdType)
		{
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
		case MODE:
			modeCmd(cmd, client);
			break;
		default:
			sendError(*client, ERR_UNKNOWNCOMMAND);
			break;
		}
	}

}