#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Server;
class Client;

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"
# include "JoinStatus.hpp"

class Channel
{
	public:
		Channel(std::string name);
		Channel(std::string name, std::string key);
		~Channel(void);
		void				addOperator(Client *client);
		void				addUser(Client *client);
		void				kickUser(Client *client);
		void				rmOperator(Client *client);
		std::string			const &getName() const;
		std::string			const &getTopic() const;
		std::string			const &getKey() const;
		std::map<std::string, Client*>	const &getUsers() const;
		std::map<std::string, Client*>	const &getOperators() const;
		std::map<std::string, Client*>	const &getInvited() const;
		size_t				const &getUserLimit() const;
		bool				const &getHasUserLimit() const;
		bool				const &getHasKey() const;
		bool				const &getIsInviteOnly() const;
		bool				const &getHasTopicRestric() const;
		void				setTopic(std::string topic);
		void				setUserLimit(size_t userLimit);
		void				setTopicRestric(bool isRestricted);
		void				setHasTopic(bool hasTopic);

		void				setKey(std::string key);
		void				setHasKey(bool hasKey);
		void				setInviteOnly(bool isInviteOnly);

		bool				isUser(Client *client);
		bool				isUser(std::string nickname);
		bool				isOperator(Client *client);
		bool				invite(Client *client);

		void				handleModes(Server *serv, Client *client, std::string const &modes, std::vector<std::string> const &modesParams);
		void				handleKeyMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode);
		void				handleOpMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode);
		void				handleLimitMode(Server *serv, Client *client, bool add, std::vector<std::string> const &params, size_t &idx, char mode);
		void				modeRPL(Client *client, char mode, bool add, std::string const &param);


		void				broadcast(std::string const &msg, Client *except);
		void				rmClient(const std::string& nickname);


		// JoinStatus	checkJoinStatus(Client *client, std::string const &key) const;
		// void		handleJoinErr(Client *client, JoinStatus status) const;

	private:
		Channel(void);
		Channel(const Channel& src);
		Channel& operator=(const Channel& rhs);
		std::string			_name;
		std::string			_topic;
		std::string			_key;
		std::map<std::string, Client*>	_users;
		std::map<std::string, Client*>	_operators;
		// std::set<Client*>	_invited;
		std::map<std::string, Client*>	_invited;
		size_t				_userLimit;
		bool				_hasKey;
		bool				_hasUserLimit;
		bool				_hasTopicRestric;
		bool				_hasTopic;
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
