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
	bool keys = cmd.params.size() > 1 ? true : false;

	std::vector<std::string> splitKeys = splitCmds(cmd.params[1]);
	Channel *channel = NULL;

	for (size_t i = 0; i < splitParams.size(); ++i)
	{
		std::string key = (keys && (i <= splitKeys.size())) ? splitKeys[i] : "";
		if (splitParams[i].empty() || splitParams[i][0] != '#')
		{
			// error ERR_NOSUCHCHANNEL
			continue;
		}

		if (_channels.find(splitParams[i]) == _channels.end())
		{
			channel = new Channel(splitParams[i], key);
			channel->addOperator(client);
			_channels[splitParams[i]] = channel;
		}
		else
		{
			channel = _channels[splitParams[i]];
		}

		JoinStatus status = channel->checkJoinStatus(client, key);

		if (status != J_OK)
		{
			channel->handleJoinErr(status);
			continue;
		}

		channel->addUser(client);
		// add RPL
	}
}
