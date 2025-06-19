#include "../includes/Server.hpp"

Server::Server(){}

Server::Server(int port, std::string &pwd): _port(port), _pwd(pwd)
{
	initServerSocket();
}

Server::~Server()
{
    close(_fd);
}

Server::Server(const Server& copy){ (void) copy; }

Server&	Server::operator=(const Server& copy)
{
	(void) copy;
	return *this;
}

int	const & Server::getFd() const { return _fd; }

int	const & Server::getPort() const { return _port; }

const std::string&	Server::getPwd() const {
	return this->_pwd;
}

void Server::initServerSocket()
{
	struct sockaddr_in server_addr;
	struct pollfd pfd;

	_name = "ircserv";
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		throw std::runtime_error("socket()");
    fcntl(_fd, F_SETFL, O_NONBLOCK);

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	if (bind(_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		close(_fd);
		throw std::runtime_error("Error: bind()");
	}

	if (listen(_fd, BACKLOG) < 0)
	{
		close(_fd);
		throw std::runtime_error("Error: listen()");
	}
	pfd.fd = _fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);

	std::cout << "Serveur waiting on port " << _port << "..." << std::endl;
}

void Server::runIRC()
{
	int status;

	while (1)
	{
		status = poll(&_pollFds[0], _pollFds.size(), -1);
		if (status == -1)
		{
			close(_fd);
			throw std::runtime_error("Error: poll()");
		}
		else if (status == 0)
			continue;

		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (!(_pollFds[i].revents & POLLIN))
				continue;

			if (_pollFds[i].fd == _fd)
				acceptNewClient();
			else
			{
				readFromSocket(_pollFds[i]);
				std::string	reply = "Hello from server!\r\n";
				send(_pollFds[i].fd, reply.c_str(), reply.length(), 0);
				// std::map<int, Client*>::iterator it = this->_clients.find(_pollFds[i].fd);
				// sendError(*it->second, 464);
				//parse msg
			}
		}
	}
}

