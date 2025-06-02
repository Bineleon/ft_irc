#include "../includes/Server.hpp"

void	Server::acceptNewClient() {
	struct sockaddr_in	client_addr;
	socklen_t	addr_len = sizeof(client_addr);
	int	client_fd = accept(this->_pollFds[0].fd, (struct sockaddr*)&client_addr, &addr_len);

	if (client_fd < 0) {
		perror("accept");
		return ;
	}

	std::cout << "A new client has been accepted.\n";

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	struct pollfd	new_pollfd;
	new_pollfd.fd = client_fd;
	new_pollfd.events = POLLIN;
	new_pollfd.revents = 0;
	this->_pollFds.push_back(new_pollfd);

	std::cout << "The new client has been added to the list of pollFds.\n";

	struct in_addr	addr = client_addr.sin_addr;
	char*	ip_cstr = inet_ntoa(addr);
	std::string	ip(ip_cstr);

	Client*	newClient = new Client(client_fd, ip);
	this->_clients.insert(std::make_pair(client_fd, newClient));

	std::cout << "The new client has been added to the list of clients.\n";
}
