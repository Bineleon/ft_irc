#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client() {
	this->_status = PASSWORD_NEEDED;
}

Client::Client(const int fd, const std::string& IP): _fd(fd), _IP(IP) {
	this->_status = PASSWORD_NEEDED;
}

const std::string&	Client::getUsername() const {
	return (this->_username);
}


void Client::sendMessage(const std::string& msg)
{
	std::string fullMsg = msg + "\r\n";
	logSend(_nickname, msg);
	appendToWriteBuffer(fullMsg);
}


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

ClientStatus const & Client::getStatus(void) const
{
	return _status;
}

void	Client::setStatus(const ClientStatus& newStatus) {
	this->_status = newStatus;
}

const Channel*	Client::getCurrentChannel() const {
	return this->_currentChannel;
}

void	Client::setCurrentChannel(Channel* channel) {
	this->_currentChannel = channel;
}

const std::string&	Client::getLastCmd() const {
	return this->_lastCmd;
}

void	Client::setLastCmd(const std::string& cmd) {
	this->_lastCmd = cmd;
}

const std::string&	Client::getNickname() const {
	return this->_nickname;
}

void	Client::setNickname(const std::string nickname) {
	this->_nickname = nickname;
}

void	Client::setUsername(const std::string username)
{
	this->_username = username;
}

const std::string&	Client::getRealname() const {
	return this->_realname;
}

void	Client::setRealname(const std::string realname) {
	this->_realname = realname;
}

int	const & Client::getFd() const { return _fd; }

void Client::appendToWriteBuffer(const std::string& data) {
	_writeBuffer += data;
}

void Client::flushWriteBuffer() {
	_writeBuffer.clear();
}

bool Client::hasDataToSend() const {
	return !_writeBuffer.empty();
}

std::string& Client::getWriteBuffer() {
	return _writeBuffer;
}
