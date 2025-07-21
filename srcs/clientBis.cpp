#include "../includes/Client.hpp"

void	Client::appendToMsgBuf(std::string msg)
{
	_msgBuffer += msg;
}

std::string &Client::getmsgBuffer(void)
{
	return _msgBuffer;
}

std::string Client::getMask(void) const
{
	return _nickname + "!" + _username + "@" + _IP;
}

void Client::setMsgBuffer(const std::string& buffer)
{
	_msgBuffer = buffer;
}
