#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void	Server::initErrorMessages() {
	this->_errorMessages[464] = std::make_pair(NOTHING, "Password incorrect");
	this->_errorMessages[461] = std::make_pair(CMD, "Not enough parameters");
	this->_errorMessages[462] = std::make_pair(NOTHING, "You may not reregister");
	this->_errorMessages[431] = std::make_pair(NOTHING, "No nickname given");
	this->_errorMessages[432] = std::make_pair(NICK, "Erroneus nickname"); // invalid characters in the nickname
	this->_errorMessages[433] = std::make_pair(NICK, "Nickname is already in use");
	this->_errorMessages[403] = std::make_pair(CHANNEL, "No such channel");
	this->_errorMessages[451] = std::make_pair(NOTHING, "You have not registered");
	this->_errorMessages[411] = std::make_pair(NOTHING, "No recipient given");
	this->_errorMessages[412] = std::make_pair(NOTHING, "No text to send");
	this->_errorMessages[481] = std::make_pair(NOTHING, "Permission denied- You're not an IRC operator");
	this->_errorMessages[502] = std::make_pair(NOTHING, "Cannot change mode for other users");
	this->_errorMessages[401] = std::make_pair(NICK, "No such nick");
	this->_errorMessages[404] = std::make_pair(CHANNEL, "Cannot send to channel");
	this->_errorMessages[421] = std::make_pair(CMD, "Unknown command");
	this->_errorMessages[441] = std::make_pair(NICK_CHANNEL, "They aren't on that channel");
	this->_errorMessages[442] = std::make_pair(CHANNEL, "You're not on that channel");
	this->_errorMessages[443] = std::make_pair(NICK_CHANNEL, "is already on channel");
	this->_errorMessages[471] = std::make_pair(CHANNEL, "Cannot join channel (+l)");
	this->_errorMessages[473] = std::make_pair(CHANNEL, "Cannot join channel (+i)");
	this->_errorMessages[474] = std::make_pair(CHANNEL, "Cannot join channel (+b)");
	this->_errorMessages[475] = std::make_pair(CHANNEL, "Cannot join channel (+k)");
	this->_errorMessages[482] = std::make_pair(CHANNEL, "You4renot channel operator");
}

void	Server::sendError(const Client& client, int error) {
	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = this->_errorMessages.find(error);

	if (it == this->_errorMessages.end()) {
		client.sendMessage("Unknown error");
		return ;
	}

	ErrorFormat	format = it->second.first;
	std::string	msg = it->second.second;

	std::ostringstream	oss;

	oss << this->_name << " " << error << " " << client.getUsername();

	switch (format)
	{
		case NOTHING:
			break;

		case CMD:
			oss << " " << client.getLastCmd();
			break;

		case NICK:
			oss << " " << client.getNickname();
			break;

		case CHANNEL:
			oss << " " << client.getCurrentChannel();
			break;

		case NICK_CHANNEL:
			oss << " " << client.getNickname() << " " << client.getCurrentChannel();
			break;

		default:
			break;
	}

	oss << ": " << msg;
	client.sendMessage(oss.str());
}

