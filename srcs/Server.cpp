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
	struct pollfd pfd;

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
	pfd.fd = _fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);

	std::cout << "Serveur waiting on port " << _port << "..." << std::endl;
}

std::map<int, std::pair<ErrorFormat, std::string>> clientMessages;

void	initErrorMessages() {
	clientMessages[464] = std::make_pair(CLIENT, "Password incorrect");
	clientMessages[461] = std::make_pair(CLIENT_CMD, "Not enough parameters");
	clientMessages[462] = std::make_pair(CLIENT, "You may not reregister");
	clientMessages[431] = std::make_pair(CLIENT, "No nickname given");
	clientMessages[432] = std::make_pair(CLIENT_NICK, "Erroneus nickname"); // invalid characters in the nickname
	clientMessages[433] = std::make_pair(CLIENT_NICK, "Nickname is already in use");
	clientMessages[403] = std::make_pair(CLIENT_CHANNEL, "No such channel");
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
			throw std::runtime_error("poll()");
		}
		else if (status == 0)
			continue;
		for (int i = 0; i < _pollFds.size(); ++i)
		{
			if ((_pollFds[i].revents & POLLIN) != 1)
				continue;
			if (_pollFds[i].fd == _fd)
			{
				//TODO : accept connection
			}
			else
			{
				//TODO : read from client
			}
		}

	}
}
