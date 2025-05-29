#include "../includes/Server.hpp"

// i = it
// read_data_from_socket(i, &poll_fds, &poll_count, server_socket);

void Server::closeClient(struct pollfd pfdClient)
{
	close(pfdClient.fd);
	std::vector<struct pollfd>::iterator it;
	for (it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == pfdClient.fd) // ou autre critère
			break;
	}
	if (it != _pollFds.end())
		_pollFds.erase(it);
}

void Server::readFromSocket(struct pollfd pfdClient)
{
	char buffer[BUFFER_SIZE];
	int bytesRead;

	std::memset(&buffer, 0, sizeof buffer);
	bytesRead = recv(pfdClient.fd, buffer, BUFFER_SIZE, 0);
	std::cout << "ICI\n";
	if (bytesRead <= 0)
	{
		if (bytesRead == 0)
			std::cout << "[" << pfdClient.fd << "] Client socket closed connection." << std::endl; 
		else
			std::cerr << "Error: recv()" << std::endl;
		closeClient(pfdClient);
	}
	else
	{
		_clients[pfdClient.fd]->appendToMsgBuf(buffer);
		std::string clientMsgBuf = _clients[pfdClient.fd]->getmsgBuffer();
		size_t pos;

		while ((pos = clientMsgBuf.find("\r\n")) != std::string::npos)
		{
			std::string toParse = clientMsgBuf.substr(0, pos);
			clientMsgBuf.erase(0, pos + 2);
			std::cout << "Msg from fd [" << pfdClient.fd << "]: " << toParse << std::endl; 
		}
		// Parse msg

	}
}