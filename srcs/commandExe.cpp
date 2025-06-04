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

void Server::joinCmd(fullCmd cmd, Client *client)
{
	std::vector<std::string> splitParams = splitCmds(cmd.params[0]);
	Channel *channel = NULL;

	for (int i = 0; i < splitParams.size(); ++i)
	{
		if (splitParams[i].empty() || splitParams[i][0] != '#')
		{
			// handle error
			continue;
		}

		if (_channels.find(splitParams[i]) == _channels.end())
		{
			channel = new Channel(splitParams[i]);
			channel->addOperator(client);
			_channels[splitParams[i]] = channel;
		}
		else
		{
			channel = _channels[splitParams[i]];
		}
		JoinStatus status = channel->checkJoinStatus(client);
		if (status != J_OK)
		{
			// TODO
			continue;
		}
		channel->addUser(client);
		// add RPL
	}
}
