#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void	initErrorMessages() {
	clientMessages[464] = std::make_pair(CLIENT, "Password incorrect");
	clientMessages[461] = std::make_pair(CLIENT_CMD, "Not enough parameters");
	clientMessages[462] = std::make_pair(CLIENT, "You may not reregister");
	clientMessages[431] = std::make_pair(CLIENT, "No nickname given");
	clientMessages[432] = std::make_pair(CLIENT_NICK, "Erroneus nickname"); // invalid characters in the nickname
	clientMessages[433] = std::make_pair(CLIENT_NICK, "Nickname is already in use");
	clientMessages[403] = std::make_pair(CLIENT_CHANNEL, "No such channel");
}

void	Server::sendError(const Client& client, int error) {
	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = clientMessages.find(error);

	if (it == clientMessages.end()) {
		this->sendMessage("Unknown error");
		return ;
	}

	ErrorFormat	format = it->second.first;
	std::string	msg = it->second.second;

	std::ostringstream	oss;

	switch (format)
	{
		case CLIENT:
			oss < ;
			break;

		default:
			break;
	}
}

