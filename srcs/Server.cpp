#include "Server.hpp"

Server::Server(){}

Server::Server(int port, std::string &pwd): _port(port), _pwd(pwd) {}

Server::~Server(){}

Server::Server(const Server& copy){ (void) copy; }

Server&	Server::operator=(const Server& copy)
{
	(void) copy;
	return *this;
}

