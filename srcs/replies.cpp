#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void Server::sendReply(Client *client, int code, std::vector<std::string> const &params, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << std::setw(3) << std::setfill('0') << code;
	
	if (!client->getNickname().empty())
		oss << " " << client->getNickname();
	for (size_t i = 0; i < params.size(); ++i)
		oss << " " << params[i];

	if (!trailing.empty())
		oss << " :" << trailing;

	client->sendMessage(oss.str());
}

void Server::sendReply(Client *client, int code, std::string const &param, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << std::setw(3) << std::setfill('0') << code;
	if (!client->getNickname().empty())
		oss << " " << client->getNickname();
	if (!param.empty())
		oss << " " << param;
	if (!trailing.empty())
		oss << " :" << trailing;
	client->sendMessage(oss.str());
}

void Server::sendReply(Client *client, int code, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << std::setw(3) << std::setfill('0') << code;
	if (!client->getNickname().empty())
		oss << " " << client->getNickname();
	if (!trailing.empty())
		oss << " :" << trailing;
	client->sendMessage(oss.str());
}

void Server::broadcastReply(Client *client, Channel *channel, int code, std::vector<std::string> const &params, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << std::setw(3) << std::setfill('0') << code << " " << client->getNickname();

	for (size_t i = 0; i < params.size(); ++i)
		oss << " " << params[i];

	if (!trailing.empty())
		oss << " :" << trailing;
	channel->broadcast(oss.str(), NULL);
}

void Server::broadcastReply(Client *client, Channel *channel, int code, std::string const &param, std::string const &trailing)
{
	std::ostringstream oss;
	oss << ":" << _name << " " << std::setw(3) << std::setfill('0') << code << " " << client->getNickname();

	if (!param.empty())
		oss << " " << param;
	if (!trailing.empty())
		oss << " :" << trailing;
	channel->broadcast(oss.str(), NULL);
}


void Server::topicRPL(Client *client, Channel *channel)
{
	if (channel->getTopic().empty())
		broadcastReply(client, channel, RPL_NOTOPIC, channel->getName(), "No topic is set");
	else
		broadcastReply(client, channel, RPL_TOPIC, channel->getName(), channel->getTopic());
}

void Server::nameRPL(Client *client, Channel *channel)
{
	std::vector<std::string> nameParams;
	nameParams.push_back("=");
	nameParams.push_back(channel->getName());

	std::ostringstream nameList;
	std::map<std::string, Client*> users = channel->getUsers();
	bool first = true;

	for (std::map<std::string, Client*>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		if (!first)
			nameList << " ";
		first = false;
		if (channel->isOperator(it->second))
			nameList << "@" << it->second->getNickname();
		else
			nameList << it->second->getNickname();
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

void Server::kickRPL(Client *source, Client *target, Channel *channel, std::string const &comment)
{
	std::ostringstream oss;
	oss << ":" << source->getMask() << " KICK " << channel->getName() << " " << target->getNickname() << " :" << comment;

	channel->broadcast(oss.str(), NULL);
}

void Server::inviteRPL(Client *client, Client *toInvite, Channel *channel)
{
	std::vector<std::string> inviteParams;
	inviteParams.push_back(toInvite->getNickname());
	inviteParams.push_back(channel->getName());
	sendReply(client, RPL_INVITING, inviteParams, "");
}

void Server::sendInvite(Client *client, Client *toInvite, Channel *channel)
{
	std::ostringstream inviteMsg;
	
	inviteMsg << ":" << client->getMask() << " INVITE " << toInvite->getNickname() << " " << channel->getName();
	toInvite->sendMessage(inviteMsg.str());
}

void Server::sendWelcome(Client *client)
{
	std::string welcomeMsg = "Welcome to the IRC " + client->getMask();
	std::vector<std::string> params;
	params.push_back(client->getNickname());
	sendReply(client, RPL_WELCOME, params, welcomeMsg);

	std::string yourHostMsg = "Your host is " + _name + ", running version 1.0";
	sendReply(client, RPL_YOURHOST, params, yourHostMsg);

	std::string createdMsg = "This server was created " + _creationDate;
	sendReply(client, RPL_CREATED, params, createdMsg);

	std::string myInfoMsg = _name + " 1.0 itkol";
	sendReply(client, RPL_MYINFO, params, myInfoMsg);
}


