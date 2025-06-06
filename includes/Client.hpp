#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>

enum ClientStatus {
	PASSWORD_NEEDED,
	NICKNAME_NEEDED,
	USERNAME_NEEDED,
	AUTHENTICATED
};

class Client
{
	private:
		int	_fd; // client file descriptor
		const std::string	_IP; // client ip adress
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		ClientStatus	_status;
		std::string	_msgBuffer;

		Client(const Client&);
		Client&	operator=(const Client&);

	public:
		Client();
		Client(const int fd, const std::string& IP);

		const std::string&	getUsername() const;
		void	setStatus(ClientStatus newStatus);

		void	sendMessage(const std::string& msg) const;
		// void	sendError(int error);

		void	PASS();
		void	NICK(const std::string nickname);
		void	setUsername(const std::string username);
		void	appendToMsgBuf(std::string msg);
		std::string const & getmsgBuffer(void) const;

        void setMsgBuffer(const std::string& buffer);

		~Client(){};
};

#endif
