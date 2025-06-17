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

