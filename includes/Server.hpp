#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "errorMsgs.hpp"

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <poll.h>
# include <exception>
# include <string>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdio>
# include <cstdlib>
# include <arpa/inet.h>
# include <algorithm>
# include <csignal>
# include <sstream>

#define BACKLOG 10
# define BUFFER_SIZE 1024

class Server
{
	private:
		Server();
		Server(const Server& copy);
		Server&	operator=(const Server& copy);

		std::string	_name;
		int _port;
		int _fd;
		std::string	_pwd;
		std::vector<struct pollfd> _pollFds;
		std::map<int, Client*> _clients;
		std::map<int, std::pair<ErrorFormat, std::string> >	_errorMessages;
		std::map<std::string, Channel*> _channels;

	public:
		Server(int port, std::string &pwd);
		~Server();
		int	const &getFd() const;
		int const &getPort() const;
		void initServerSocket();
		void runIRC();

		void readFromSocket(struct pollfd pfdClient);
		void closeClient(struct pollfd pfdClient);

		void acceptNewClient();

		void cleanAll();

		void initErrorMessages();
		void sendError(const Client& client, int error);
		void executeCmd(fullCmd cmd, Client client);
		void joinCmd(fullCmd cmd, Client *client);

		// void	handleCommands(Client *client, const std::string& cmd, const std::vector<std::string>& args);
		// void	PASS(Client *client, const std::string pass);
};

#endif
