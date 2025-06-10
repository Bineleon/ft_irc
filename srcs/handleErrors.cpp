#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

std::map<int, std::pair<ErrorFormat, std::string> > clientMessages;

void	Server::initErrorMessages() {
	clientMessages[464] = std::make_pair(NOTHING, "Password incorrect");
	clientMessages[461] = std::make_pair(CMD, "Not enough parameters");
	clientMessages[462] = std::make_pair(NOTHING, "You may not reregister");
	clientMessages[431] = std::make_pair(NOTHING, "No nickname given");
	clientMessages[432] = std::make_pair(NICK, "Erroneus nickname"); // invalid characters in the nickname
	clientMessages[433] = std::make_pair(NICK, "Nickname is already in use");
	clientMessages[403] = std::make_pair(CHANNEL, "No such channel");
	clientMessages[ERR_BANNEDFROMCHAN] = std::make_pair(CHANNEL, "Cannot join channel (+b)");
}

void	Server::sendError(const Client& client, int error) {
	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = clientMessages.find(error);

	if (it == clientMessages.end()) {
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
			oss << " " ;

		default:
			break;
	}
}
