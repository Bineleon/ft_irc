#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
	private:
		// int	_fd; // client file descriptor
		// std::string	_IP; // client ip adress
		// std::string	_nickname;
		// std::string	_username;
		// std::string	_realname;
		// std::string	_hostname;

	public:
		Client();
		Client(const Client& copy);
		Client&	operator=(const Client& copy);
		~Client();
};

#endif
