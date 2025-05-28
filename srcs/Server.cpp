#include "../includes/Server.hpp"

Server::Server(){}

Server::Server(int port, std::string &pwd): _port(port), _pwd(pwd)
{
	initServerSocket();
}

Server::~Server() {}

Server::Server(const Server& copy){ (void) copy; }

Server&	Server::operator=(const Server& copy)
{
	(void) copy;
	return *this;
}

int	const & Server::getFd() const { return _fd; }

int	const & Server::getPort() const { return _port; }



void Server::initServerSocket()
{
	struct sockaddr_in server_addr;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::runtime_error("socket()");
	
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		close(_fd);
		throw std::runtime_error("bind()");
	}

	if (listen(_fd, BACKLOG) < 0)
	{
		close(_fd);
		throw std::runtime_error("listen()");
	}
}

