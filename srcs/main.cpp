// #include "../includes/minitcp.hpp"
#include "../includes/Server.hpp"

// /connect 127.0.0.1 6667 pass

Server* gServ = NULL;

int	main(int ac, char **av)
{
	long port;
	const char *sPort = av[1];
	std::string pwd;
	char * pEnd;

	if (ac != 3)
	{
		std::cerr << "Format: <port> <password>" << std::endl;
		return (1);
	}

	pwd = av[2];
	port = std::strtol(sPort, &pEnd, 10);
	try
	{
		Server serv(port, pwd);
		gServ = &serv;
		handleSignals();
		serv.runIRC();
	}
	catch(const std::exception& e)
	{
        std::cerr << e.what() << '\n';
	}
	return 0;
}

// int	main(int ac, char **av)
// {
// 	int	client_fd;
// 	struct sockaddr_in client_addr;
// 	char	buffer[BUFFER_SIZE];
// 	socklen_t	addr_len = sizeof(client_addr);
// 	int port;
// 	std::string pwd;

// 	if (ac != 3)
// 	{
// 		std::cerr << "Format: <port> <password>" << std::endl;
// 		return (1);
// 	}

// 	pwd = av[2];
// 	port = std::atoi(av[1]);
// 	try
// 	{
// 		Server serv(port, pwd);

// 		client_fd = accept(serv.getFd(), (struct sockaddr*)&client_addr, &addr_len);
// 		if (client_fd < 0) {
// 			std::perror("accept");
// 			close(serv.getFd());
// 			return 1;
// 		}

// 		std::memset(buffer, 0, BUFFER_SIZE);
// 		ssize_t	bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
// 		if (bytes_received > 0) {
// 			std::cout << "Message received: " << buffer << std::endl;

// 			std::string	reply = "Hello from server!\r\n";
// 			send(client_fd, reply.c_str(), reply.length(), 0);
// 		}

// 		close(client_fd);
// 		close(serv.getFd());
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}


// 	return 0;
// }


// int	main(int ac, char **av) {
// 	int	client_fd;
// 	struct sockaddr_in client_addr;
// 	// int	server_fd, client_fd;
// 	// struct sockaddr_in server_addr, client_addr;
// 	char	buffer[BUFFER_SIZE];
// 	socklen_t	addr_len = sizeof(client_addr);

// 	Server serv();

// 	// server_fd = socket(AF_INET, SOCK_STREAM, 0);
// 	// if (server_fd == -1) {
// 	// 	std::perror("socket");
// 	// 	return 1;
// 	// }

// 	// std::memset(&server_addr, 0, sizeof(server_addr));
// 	// server_addr.sin_family = AF_INET;
// 	// server_addr.sin_addr.s_addr = INADDR_ANY;
// 	// server_addr.sin_port = htons(PORT);

// 	// if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
// 	// 	std::perror("bind");
// 	// 	close(server_fd);
// 	// 	return 1;
// 	// }

// 	// if (listen(server_fd, 5) < 0) {
// 	// 	std::perror("listen");
// 	// 	close(server_fd);
// 	// 	return 1;
// 	// }

// 	std::cout << "Serveur waiting on port " << PORT << "..." << std::endl;

// 	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
// 	if (client_fd < 0) {
// 		std::perror("accept");
// 		close(server_fd);
// 		return 1;
// 	}

// 	std::memset(buffer, 0, BUFFER_SIZE);
// 	ssize_t	bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
// 	if (bytes_received > 0) {
// 		std::cout << "Message received: " << buffer << std::endl;

// 		std::string	reply = "Hello from server!\r\n";
// 		send(client_fd, reply.c_str(), reply.length(), 0);
// 	}

// 	close(client_fd);
// 	close(server_fd);

// 	return 0;
// }
