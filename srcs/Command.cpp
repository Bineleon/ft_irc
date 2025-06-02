#include "../includes/Command.hpp"

std::string toUpper(std::string &str)
{
	std::string up = str;
	for (int i = 0; i < up.length(); ++i)
		std::toupper(up[i]);
	return up;
}

fullCmd parseCmd(std::string const & msg)
{
	fullCmd	cmd;
	std::istringstream s (msg);
	std::string word;
	std::string restOfTrail;

	if (!(s >> cmd.verb))
		return cmd;
	cmd.verb = toUpper(cmd.verb);
	
	while (s >> word)
	{
		
		if (word[0] == ':')
		{
			getline(s, restOfTrail);
			cmd.trailing = word.substr(1) + restOfTrail;
			break;
		}
		std::cout << "word: " << word << std::endl;
		cmd.params.push_back(word);
	}
	return cmd;
}

void printCmd(fullCmd cmd)
{
	std::cout << "Verb: " << cmd.verb << std::endl;
	for (size_t i = 0; i < cmd.params.size(); ++i)
		std::cout << "Param[" << i << "]: " << cmd.params[i] << std::endl;
	std::cout << "Trailing: " << cmd.trailing << std::endl;
}