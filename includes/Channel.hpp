#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>
#include "Client.hpp"

class Channel
{
	public:
		Channel(void);
		~Channel(void);
		
	private:
		Channel(const Channel& src);
		Channel& operator=(const Channel& rhs);
		std::string			_name;
		std::string			_topic;
		std::string			_pwd;
		std::string			_description;
		std::set<Client*>	_users;
		std::set<Client*>	_operators;
		size_t				_userLimit;
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
