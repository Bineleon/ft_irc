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
			printCmd(cmd);
			std::map<int, Client*>::iterator it = this->_clients.find(pfdClient.fd);
			execCmd(cmd, *this, *it->second);
		}
	}
}


bool Server::chanIsOnServer(std::string chanName)
{
	if (_channels.find(chanName) == _channels.end())
		return false;
	return true;
}
