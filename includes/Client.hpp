#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include "Channel.hpp"

class Channel;

enum ClientStatus {
	PASSWORD_NEEDED,
	NICKNAME_NEEDED,
	USERNAME_NEEDED,
	AUTHENTICATED
};

class Client
{
	private:
		int					_fd;
		const std::string	_IP;
		std::string			_nickname;
		std::string			_username;
		std::string			_realname;
		std::string			_msgBuffer;
		ClientStatus		_status;
		Channel*			_currentChannel;
		std::string			_lastCmd;

		Client(const Client&);
		Client&	operator=(const Client&);

	public:
		Client();
		Client(const int fd, const std::string& IP);

		const std::string&	getUsername() const;
		const std::string&	getNickname() const;
		void	setStatus(const ClientStatus& newStatus);
		const Channel*	getCurrentChannel() const;
		void	setCurrentChannel(Channel* channel);
		const std::string&	getLastCmd() const;
		void	setLastCmd(const std::string& cmd);
		const std::string&	getNickname() const;

		void				sendMessage(const std::string& msg) const;
		// void	sendError(int error);

		void				PASS();
		void				NICK(const std::string nickname);
		void				setUsername(const std::string username);
		void				appendToMsgBuf(std::string msg);
		std::string const &getmsgBuffer(void) const;
		// std::string const &getMask(void) const;
		ClientStatus const &getStatus(void) const;

		void setMsgBuffer(const std::string& buffer);

		~Client(){};
};

#endif
