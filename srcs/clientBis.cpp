#include "../includes/Client.hpp"

void	Client::appendToMsgBuf(std::string msg)
{
	_msgBuffer += msg;
}

std::string const &Client::getmsgBuffer(void) const
{
	return _msgBuffer;
}

std::string const &Client::getMask(void) const
{
	return _nickname + "!" + _username + "@localhost";
}

void Client::setMsgBuffer(const std::string& buffer)
{
	_msgBuffer = buffer;
}

const std::string&	Client::getNickname() const
{
	return (_nickname);
}