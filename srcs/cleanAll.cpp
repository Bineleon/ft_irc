#include "../includes/Server.hpp"

void	Server::cleanAll()
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second)
		{
			close(it->first);
			delete it->second;
		}
	}
	_clients.clear();
	_nickClients.clear();
	
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second)
		delete it->second;
	}
	_channels.clear();
	// _pollFds.clear();
	std::vector<struct pollfd>().swap(_pollFds);

	if (_fd > 0)
		close(_fd);
}
