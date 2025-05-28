#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"

# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <poll.h>
# include <exception>
# include <string>
# include <cstring>
# include <unistd.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdio>

#define BACKLOG 10

class Server
{
	private:
		Server();
		Server(const Server& copy);
		Server&	operator=(const Server& copy);

		int _port;
		int _fd;
		std::string	_pwd;
		std::vector<struct pollfd> _pollFds;
	
	public:
		Server(int port, std::string &pwd);
		~Server();
		void initServerSocket();
};

#endif
