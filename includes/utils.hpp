# ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <limits>
# include "Server.hpp"
# include "Command.hpp"

bool	convertToInt(std::string const &str, int &result);
bool	checkNeedMoreParams(fullCmd cmd);
bool	isValidChanName(std::string const &chanName);
bool	isValidNickname(const std::string& nickname);
void	debug(std::string msg);
void	handleSignals();
void    logInfo(const std::string& msg);
void    logRecv(const std::string& from, const std::string& msg);
void    logSend(const std::string& to, const std::string& msg);
void    logError(const std::string& msg);

# endif
