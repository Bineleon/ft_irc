#include "../includes/Server.hpp"

Server::Server(){}

Server::Server(int port, std::string &pwd): _port(port), _pwd(pwd)
{
	initServerSocket();
	initCreationDate();
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
	while (1)
	{
		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			int fd = _pollFds[i].fd;
			_pollFds[i].events = POLLIN;
			if (_clients.find(fd) != _clients.end() && _clients[fd]->hasDataToSend())
				_pollFds[i].events |= POLLOUT;
		}

		int status = poll(&_pollFds[0], _pollFds.size(), -1);
		if (status == -1)
			throw std::runtime_error("Error: poll()");
		else if (status == 0)
			continue;
			
		for (int i = _pollFds.size() - 1; i >= 0; --i)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _fd)
					acceptNewClient();
				else
					readFromSocket(_pollFds[i]);
			}
			if (_pollFds[i].revents & POLLOUT)
				writeToSocket(_pollFds[i]);
		}
	}

}


void Server::initCreationDate()
{
	// std::time_t now = std::time(NULL);
	// _creationDate = std::string(std::ctime(&now));
	// _creationDate.erase(_creationDate.find_last_not_of("\n") + 1);
	_creationDate = "today";
}

void Server::writeToSocket(struct pollfd pfd)
{
	Client* client = _clients[pfd.fd];
	if (!client || !client->hasDataToSend())
		return;

	std::string& buffer = client->getWriteBuffer();
	ssize_t sent = send(pfd.fd, buffer.c_str(), buffer.size(), 0);

	if (sent < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		std::cerr << "send() failed for client " << client->getNickname() << std::endl;
		closeClient(pfd);
		return;
	}

	if ((size_t)sent < buffer.size())
		buffer.erase(0, sent);
	else
		client->flushWriteBuffer();
}
