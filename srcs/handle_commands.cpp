#include "../includes/Server.hpp"

void	Server::PASS(Client *client, const std::string pass) {
	if (pass.empty())
		client->sendMessage();
}

void	Server::handle_commands(Client* client, const std::string& cmd, const std::vector<std::string>& args) {
	if (cmd == "PASS")
		PASS()
}
