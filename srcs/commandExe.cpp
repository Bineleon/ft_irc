#include "../includes/Server.hpp"

void Server::joinCmd(fullCmd cmd, Client client)
{
	// if , in param, split param
	if (cmd.params[i].empty() || cmd.params[i][0] != '#')
	{
		// handle error
		return;
	}

	if (_channels.find(cmd.params[i]) == _channels.end())
	{
		// Create channel
		// add client as operator
	}
	else
	{
		// Add client as user
	}
}
