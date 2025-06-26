# ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <limits>
# include "Server.hpp"
# include "Command.hpp"

bool convertToInt(std::string const &str, int &result);
bool checkNeedMoreParams(fullCmd cmd);
bool isValidChanName(std::string const &chanName);

# endif