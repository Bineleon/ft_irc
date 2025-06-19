#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void Server::sendReply(Client *client, int code, std::vector<std::string> &params, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << code << " " << client->getNickname();

	for (int i = 0; i < params.size(); ++i)
		oss << " " << params[i];

	if (!trailing.empty())
		oss << " :" << trailing;

	client->sendMessage(oss.str());
}

//  :dan-!d@localhost JOIN #test    ; dan- is joining the channel #test

void Server::topicRPL(Client *client, Channel *channel)
{
	std::vector<std::string> topicParams;
	topicParams.push_back(channel->getName());
	if (channel->getTopic().empty())
		sendReply(client, RPL_NOTOPIC, topicParams, "No topic is set");
	else
		sendReply(client, RPL_TOPIC, topicParams, channel->getTopic());
}

void Server::nameRPL(Client *client, Channel *channel)
{
	std::vector<std::string> nameParams;
	nameParams.push_back("=");
	nameParams.push_back(channel->getName());

	std::ostringstream nameList;
	std::map<std::string, Client*> users = channel->getUsers();
	for (std::map<std::string, Client*>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		if (channel->isOperator(it->second))
			nameList << "@" << it->second->getNickname();
		else
			nameList << it->second->getNickname();

		if (std::next(it) != users.end())
			nameList << " ";
	}
	sendReply(client, RPL_NAMREPLY, nameParams, nameList.str());
}

void Server::eofNamesRPL(Client *client, Channel *channel)
{
	std::vector<std::string> eofNamesParams;
	eofNamesParams.push_back(channel->getName());
	sendReply(client, RPL_ENDOFNAMES, eofNamesParams, "End of /NAMES list");
}

void Server::joinRPLs(Client *client, Channel *channel)
{
	std::ostringstream joinMsg;
	joinMsg << ":" << client->getMask() << " JOIN " << channel->getName();
	channel->broadcast(joinMsg.str(), NULL);
	
	topicRPL(client, channel);
	nameRPL(client, channel);
	eofNamesRPL(client, channel);
}