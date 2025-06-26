#include "../includes/utils.hpp"

bool convertToInt(std::string const &str, int &result)
{
	long long tmp;
	std::stringstream ss(str);

	if (str.empty())
		return false;

	ss >> tmp;
	if (ss.fail() || ss.eof())
		return false;

	if (tmp > std::numeric_limits<int>::min() || tmp < std::numeric_limits<int>::max())
		return false;

	result = static_cast<int>(tmp);
	return true;
}

bool checkNeedMoreParams(fullCmd cmd)
{
	if (cmd.params.empty() || cmd.params[0].empty())
		return true;
	return false;
}

bool isValidChanName(std::string const &chanName)
{
	if (chanName.empty() || chanName[0] != '#')
		return false;
	return true;
}

bool	isValidNickname(const std::string& nickname) {
	std::size_t	found = nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]{}\\|");
	if (found != std::string::npos)
		return false;
	return true;
}
