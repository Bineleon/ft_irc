#include "../includes/Client.hpp"

void	Client::appendToMsgBuf(std::string msg)
{
	_msgBuffer + msg;
}

std::string const & Client::getmsgBuffer(void) const
{
	return _msgBuffer;
}