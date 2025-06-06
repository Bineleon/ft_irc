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

		default:
			break;
	}

	oss << ": " << msg;
	client.sendMessage(oss.str());
}

