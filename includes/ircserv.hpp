#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include "client.hpp"

# include <iostream>
# include <vector>

class Server
{
	private:

	public:
		Server();
		Server(const Server& copy);
		Server&	operator=(const Server& copy);
		~Server();
};

#endif
