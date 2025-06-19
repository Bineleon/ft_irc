#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "errorMsgs.hpp"
# include "replies.hpp"
#include "utils.hpp"


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
# include <iomanip>

# define BACKLOG 10
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
		std::map<std::string, Client*> _nickClients;
		std::map<int, std::pair<ErrorFormat, std::string> >	_errorMessages;
		std::map<std::string, Channel*> _channels;

	public:
		Server(int port, std::string &pwd);
		~Server();
		int	const &getFd() const;
		int const &getPort() const;
		const std::string&	getPwd() const;
		void initServerSocket();
		void runIRC();

		void readFromSocket(struct pollfd pfdClient);
		void closeClient(struct pollfd pfdClient);

		void acceptNewClient();

		void cleanAll();

		void initErrorMessages();
		void sendError(const Client& client, int error);
		void executeCmd(fullCmd cmd, Client client);
		CMD_TYPE checkCMD(fullCmd cmd);

		void privmsgCmd(fullCmd cmd, Client *client);
		void handleChanPrivmsg(fullCmd cmd, Client *client);
		void handleUserPrivmsg(fullCmd cmd, Client *client);
		std::string buildPrivmsg(fullCmd cmd, Client *client);

		void joinCmd(fullCmd cmd, Client *client);
		JoinStatus	checkJoinStatus(Channel *channel, Client *client, std::string const &key) const;
		void handleJoinErr(Client *client, JoinStatus status);
		Channel* handleJoinChan(Client *client, std::string const &key, std::string chanName);

		void topicRPL(Client *client, Channel *channel);
		void nameRPL(Client *client, Channel *channel);
		void eofNamesRPL(Client *client, Channel *channel);

		void kickCmd(fullCmd cmd, Client *client);
		void inviteCmd(fullCmd cmd, Client *client);
		void topicCmd(fullCmd cmd, Client *client);
		void modeCmd(fullCmd cmd, Client *client);

		void sendReply(Client *client, int code, std::vector<std::string> &params, std::string const &trailing);
		void joinRPLs(Client *client, Channel *channel);

		bool chanIsOnServer(std::string chanName);

		void	passCmd(fullCmd cmd, Client *client);
		void	passNick(fullCmd cmd, Client *client);

		// void	handleCommands(Client *client, const std::string& cmd, const std::vector<std::string>& args);
		// void	PASS(Client *client, const std::string pass);
};

#endif
