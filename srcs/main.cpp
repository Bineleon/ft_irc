#include "../includes/Server.hpp"

// /connect 127.0.0.1 6667 pass

Server* gServ = NULL;

int	main(int ac, char **av)
{
	long port;
	const char *sPort = av[1];
	std::string pwd;
	char * pEnd;

	if (ac != 3)
	{
		std::cerr << RED << "Error format: <port> <password>" << RESET << std::endl;
		return (1);
	}

	pwd = av[2];
	port = std::strtol(sPort, &pEnd, 10);
	if (*pEnd || port < 1024 || port > 65535)
	{
		std::cerr << RED << "Error: Invalid port, use a port between 1024 and 65535" << RESET << std::endl;
		return (1);
	}
	try
	{
		Server serv(port, pwd);
		gServ = &serv;
		handleSignals();
		serv.runIRC();
	}
	catch(const std::exception& e)
	{
        std::cerr << e.what() << '\n';
	}
	return 0;
}
