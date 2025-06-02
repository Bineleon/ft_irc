#include "../includes/Server.hpp"

void	handle_signals() {
	std::signal(SIGINT, handle_signal);
	std::signal(SIGTERM, handle_signal);
	std::signal(SIGPIPE, SIG_IGN);
}

void	handle_signal(int signal) {
	if (signal == SIGINT || signal == SIGTERM) {
		std::cout << "\nSignal received, stopping server...\n";
		// appeler les fonctions pour clean les sockets liberer la memoire etc
	}
	std::exit(0);
}
