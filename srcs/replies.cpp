#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

void Server::sendReply(const Client& client, int code)
{
	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = clientMessages.find(code);

	if (it == clientMessages.end())
    {
		client.sendMessage("Unknown reply");
		return ;
	}

	std::ostringstream oss;
	oss << ":" << _name << " " << code << " ";

	ErrorFormat type = it->second.first;
	std::string msg = it->second.second;

	switch (type)
    {
		// TODO
	}

	oss << msg;
	client.sendMessage(oss.str());
}
