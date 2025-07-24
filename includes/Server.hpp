#ifndef SERVER_HPP
# define SERVER_HPP

class Channel;
class Client;

# include "errorMsgs.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Command.hpp"
# include "replies.hpp"
# include "utils.hpp"
# include "JoinStatus.hpp"

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
# include <ctime>
# include <cerrno>

# define BACKLOG 10
# define BUFFER_SIZE 1024

enum ErrorFormat {
	NOTHING,
	NICK_ERR,
	CMD,
	CHANNEL,
	NICK_CHANNEL
};

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
		std::map<std::string, Channel*> _channels;
		std::string _creationDate;

	public:
		Server(int port, std::string &pwd);
		~Server();
		int	const			&getFd() const;
		int const			&getPort() const;
		const std::string	&getPwd() const;
		void				initServerSocket();
		void				runIRC();

		void				readFromSocket(struct pollfd pfdClient);
		void				writeToSocket(struct pollfd pfd);

		void				closeClient(struct pollfd pfdClient);
		void				closeClient(Client *client);

		void				rmClientFromChannels(Client *clientToRm);

		void				acceptNewClient();

		void				cleanAll();
		void				executeCmd(fullCmd cmd, Client *client);
		CMD_TYPE			checkCMD(fullCmd cmd);

		void				privmsgCmd(fullCmd cmd, Client *client);
		void				handleChanPrivmsg(fullCmd cmd, Client *client);
		void				handleUserPrivmsg(fullCmd cmd, Client *client);
		std::string			buildPrivmsg(fullCmd cmd, Client *client);

		void				joinCmd(fullCmd cmd, Client *client);
		JoinStatus			checkJoinStatus(Channel *channel, Client *client, std::string const &key) const;
		void				handleJoinErr(Client *client, JoinStatus status, Channel *channel);
		Channel*			handleJoinChan(Client *client, std::string const &key, std::string chanName);

		void				kickCmd(fullCmd cmd, Client *client);
		void				inviteCmd(fullCmd cmd, Client *client);
		void				topicCmd(fullCmd cmd, Client *client);
		void				modeCmd(fullCmd cmd, Client *client);
		void				partCmd(fullCmd cmd, Client* client);
		void				quitCmd(fullCmd cmd, Client* client);
		void				passCmd(fullCmd cmd, Client *client);
		void				nickCmd(fullCmd cmd, Client *client);
		void				userCmd(fullCmd cmd, Client *client);
		void				pongCmd(fullCmd cmd, Client *client);

		
		void				sendReply(Client *client, int code, std::vector<std::string> const &params, std::string const &trailing);
		void				sendReply(Client *client, int code, std::string const &param, std::string const &trailing);

		void				topicRPL(Client *client, Channel *channel);
		void				nameRPL(Client *client, Channel *channel);
		void				eofNamesRPL(Client *client, Channel *channel);
		void				joinRPLs(Client *client, Channel *channel);
		void				kickRPL(Client *source, Client *target, Channel *channel, std::string const &comment);
		void				inviteRPL(Client *client, Client *toInvite, Channel *channel);
		void				sendInvite(Client *client, Client *toInvite, Channel *channel);
		void				sendWelcome(Client *client);
		void				sendNickMsg(std::string oldMask, Client *client);

		void				initCreationDate();

		bool				chanIsOnServer(std::string chanName);
		bool				checkAuthenticated(Client *client);

};

# define RESET "\033[0m"
# define SMRED "\033[0;31m"
# define SMGREEN "\033[0;32m"
# define SMYELLOW "\033[0;33m"
# define SMBLUE "\033[0;34m"
# define SMMAGENTA "\033[0;35m"
# define SMCYAN "\033[0;36m"

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"

#endif
