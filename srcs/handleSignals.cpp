#include "../includes/Server.hpp"

extern Server* gServ;

void	handleSignal(int signal) {
	if (signal == SIGINT || signal == SIGTERM) {
		std::cout << "\nSignal received, stopping server...\n";
		if (gServ)
			gServ->cleanAll();
		std::exit(signal);
	}
}

void	handleSignals()
{
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);
	std::signal(SIGPIPE, SIG_IGN);
}
