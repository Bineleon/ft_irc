#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client() {
	this->_status = PASSWORD_NEEDED;
}

Client::Client(const int fd, const std::string& IP): _fd(fd), _IP(IP) {
	this->_status = PASSWORD_NEEDED;
}

Client& Client::operator=(const Client& rhs)
{
    (void) rhs;
    return *this;
}

Client::~Client(){};

const std::string&	Client::getUsername() const {
	return (this->_username);
}

void	Client::appendToMsgBuf(std::string msg)
{
	_msgBuffer += msg;
}

std::string &Client::getmsgBuffer(void)
{
	return _msgBuffer;
}

void Client::setMsgBuffer(const std::string& buffer)
{
	_msgBuffer = buffer;
}

std::string Client::getMask(void) const
{
	return _nickname + "!" + _username + "@" + _IP;
}

void Client::sendMessage(const std::string& msg)
{
	std::string fullMsg = msg + "\r\n";
	logSend(_nickname, msg);
	appendToWriteBuffer(fullMsg);
}

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
