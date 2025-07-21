#include "../includes/Server.hpp"

extern Server* gServ;

void	handleSignals()
{
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);
	std::signal(SIGPIPE, SIG_IGN);
}

void	handleSignal(int signal) {
	if (signal == SIGINT || signal == SIGTERM) {
		std::cout << "\nSignal received, stopping server...\n";
		// appeler les fonctions pour clean les sockets liberer la memoire etc
		if (gServ)
			gServ->cleanAll();
		
	}
	std::exit(signal);
}
