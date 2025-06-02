#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client() {
	this->_status = PASSWORD_NEEDED;
}

Client::Client(const int fd, const std::string& IP): _fd(fd), _IP(IP) {
	this->_status = PASSWORD_NEEDED;
}

// void	Client::sendMessage(const std::string& msg) {
// 	std::string	fullMsg = msg + "\r\n";
// 	send(this->_fd, fullMsg.c_str(), fullMsg.length(), 0);
// }

// void	Client::sendError(int error) {
// 	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = clientMessages.find(error);

// 	if (it == clientMessages.end()) {
// 		this->sendMessage("Unknown error");
// 		return ;
// 	}

// 	ErrorFormat	format = it->second.first;
// 	std::string	msg = it->second.second;

// 	std::ostringstream	oss;

// 	switch (format)
// 	{
// 		case CLIENT:
// 			sendMessage(this->)
// 			break;

// 		default:
// 			break;
// 	}
// }

// void	Client::PASS() {

// 	this->_status = NICKNAME_NEEDED;
// }

// void	Client::NICK(const std::string nickname) {
// 	if (this->_status == PASSWORD_NEEDED) {
// 		sendMessage("Please enter the password using the PASS command before configuring your nickname");
// 		return ;
// 	}

// 	this->_nickname = nickname;
// 	sendMessage("Nickname has been set to " + nickname);

// 	if (this->_status == NICKNAME_NEEDED)
// 		this->_status = USERNAME_NEEDED;
// }

// void	Client::setUsername(const std::string username) {
// 	if (this->_status == PASSWORD_NEEDED) {
// 		sendMessage("Please enter the password using the PASS command before configuring your username");
// 		return ;
// 	}

// 	if (this->_status == NICKNAME_NEEDED) {
// 		sendMessage("Please enter a nickname using the NICK command before configuring your username");
// 		return ;
// 	}

// 	this->_username = username;
// 	sendMessage("Username has been set to " + username);

// 	if (this->_status == USERNAME_NEEDED)
// 		this->_status = AUTHENTICATED;
// }

