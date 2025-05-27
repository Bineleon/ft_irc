#include "minitcp.hpp"

int	main() {
	int	server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	char	buffer[BUFFER_SIZE];
	socklen_t	addr_len = sizeof(client_addr);

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		std::perror("socket");
		return 1;
	}

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::perror("bind");
		close(server_fd);
		return 1;
	}

	if (listen(server_fd, 5) < 0) {
		std::perror("listen");
		close(server_fd);
		return 1;
	}

	std::cout << "Serveur waiting on port " << PORT << "..." << std::endl;

	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
	if (client_fd < 0) {
		std::perror("accept");
		close(server_fd);
		return 1;
	}

	std::memset(buffer, 0, BUFFER_SIZE);
	ssize_t	bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_received > 0) {
		std::cout << "Message received: " << buffer << std::endl;

		std::string	reply = "Hello from server!\r\n";
		send(client_fd, reply.c_str(), reply.length(), 0);
	}

	close(client_fd);
	close(server_fd);

	return 0;
}
