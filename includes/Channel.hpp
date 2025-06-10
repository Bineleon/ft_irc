#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>
#include <map>
#include "Client.hpp"

class Client;

enum JoinStatus
{
	J_OK,
	J_FULL,
	J_INVIT_O,
	J_BANNED,
	J_BAD_K,
	J_ALRDYIN
};

class Channel
{
	public:
		Channel(std::string name);
		Channel(std::string name, std::string key);
		~Channel(void);
		void		addOperator(Client *client);
		void		addUser(Client *client);
		void		kickUser(Client *client);
		std::string			const &getName() const;
		std::string			const &getTopic() const;
		std::string			const &getKey() const;
		std::map<std::string, Client*>	const &getUsers() const;
		std::set<Client*>	const &getOperators() const;
		std::set<Client*>	const &getBanned() const;
		std::set<Client*>	const &getInvited() const;
		size_t				const &getUserLimit() const;
		bool				const &getHasUserLimit() const;
		bool				const &getHasKey() const;
		bool				const &getIsInviteOnly() const;
		// JoinStatus	checkJoinStatus(Client *client, std::string const &key) const;
		// void		handleJoinErr(Client *client, JoinStatus status) const;

	private:
		Channel(void);
		Channel(const Channel& src);
		Channel& operator=(const Channel& rhs);
		std::string			_name;
		std::string			_topic;
		std::string			_key;
		// std::set<Client*>	_users;
		std::map<std::string, Client*>	_users;
		std::set<Client*>	_operators;
		std::set<Client*>	_banned;
		std::set<Client*>	_invited;
		size_t				_userLimit;
		bool				_hasUserLimit;
		bool				_hasKey;
		bool				_isInviteOnly;
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
