#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

// void	Server::initErrorMessages() {
// 	this->_errorMessages[ERR_PASSWDMISMATCH]   = std::make_pair(NOTHING, "Password incorrect");
// 	this->_errorMessages[ERR_NEEDMOREPARAMS]   = std::make_pair(CMD, "Not enough parameters");
// 	this->_errorMessages[ERR_ALREADYREGISTERED]= std::make_pair(NOTHING, "You may not reregister");
// 	this->_errorMessages[ERR_NONICKNAMEGIVEN]  = std::make_pair(NOTHING, "No nickname given");
// 	this->_errorMessages[ERR_ERRONEUSNICKNAME] = std::make_pair(NICK_ERR, "Erroneus nickname");
// 	this->_errorMessages[ERR_NICKNAMEINUSE]    = std::make_pair(NICK_ERR, "Nickname is already in use");
// 	this->_errorMessages[ERR_NOSUCHCHANNEL]    = std::make_pair(CHANNEL, "No such channel");
// 	this->_errorMessages[ERR_NOTREGISTERED]    = std::make_pair(NOTHING, "You have not registered");
// 	this->_errorMessages[ERR_NORECIPIENT]      = std::make_pair(NOTHING, "No recipient given");
// 	this->_errorMessages[ERR_NOTEXTTOSEND]     = std::make_pair(NOTHING, "No text to send");
// 	this->_errorMessages[ERR_NOPRIVILEGES]     = std::make_pair(NOTHING, "Permission denied- You're not an IRC operator");
// 	this->_errorMessages[ERR_USERSDONTMATCH]   = std::make_pair(NOTHING, "Cannot change mode for other users");
// 	this->_errorMessages[ERR_NOSUCHNICK]       = std::make_pair(NICK_ERR, "No such nick");
// 	this->_errorMessages[ERR_CANNOTSENDTOCHAN] = std::make_pair(CHANNEL, "Cannot send to channel");
// 	this->_errorMessages[ERR_UNKNOWNCOMMAND]   = std::make_pair(CMD, "Unknown command");
// 	this->_errorMessages[ERR_USERNOTINCHANNEL] = std::make_pair(NICK_CHANNEL, "They aren't on that channel");
// 	this->_errorMessages[ERR_NOTONCHANNEL]     = std::make_pair(CHANNEL, "You're not on that channel");
// 	this->_errorMessages[ERR_USERONCHANNEL]    = std::make_pair(NICK_CHANNEL, "is already on channel");
// 	this->_errorMessages[ERR_CHANNELISFULL]    = std::make_pair(CHANNEL, "Cannot join channel (+l)");
// 	this->_errorMessages[ERR_INVITEONLYCHAN]   = std::make_pair(CHANNEL, "Cannot join channel (+i)");
// 	this->_errorMessages[ERR_BANNEDFROMCHAN]   = std::make_pair(CHANNEL, "Cannot join channel (+b)");
// 	this->_errorMessages[ERR_BADCHANNELKEY]    = std::make_pair(CHANNEL, "Cannot join channel (+k)");
// 	this->_errorMessages[ERR_CHANOPRIVSNEEDED] = std::make_pair(CHANNEL, "You're not channel operator");
// 	this->_errorMessages[ERR_UNKNOWNMODE]		= std::make_pair(CHANNEL, "<client> <modechar> :is unknown mode char to me");
// 	this->_errorMessages[ERR_NOORIGIN]			= std::make_pair(NOTHING, "No origin specified");

// }

// void	Server::sendError(const Client& client, int error) {
// 	std::map<int, std::pair<ErrorFormat, std::string> >::iterator it = this->_errorMessages.find(error);

// 	if (it == this->_errorMessages.end()) {
// 		client.sendMessage("Unknown error");
// 		return ;
// 	}

// 	ErrorFormat	format = it->second.first;
// 	std::string	msg = it->second.second;

// 	std::ostringstream	oss;

// 	oss << this->_name << " " << error << " " << client.getUsername();

// 	switch (format)
// 	{
// 		case NOTHING:
// 			break;

// 		case CMD:
// 			oss << " " << client.getLastCmd();
// 			break;

// 		case NICK_ERR:
// 			oss << " " << client.getNickname();
// 			break;

// 		case CHANNEL:
// 			oss << " " << client.getCurrentChannel();
// 			break;

// 		case NICK_CHANNEL:
// 			oss << " " << client.getNickname() << " " << client.getCurrentChannel();
// 			break;

// 		default:
// 			break;
// 	}

// 	oss << ": " << msg;
// 	client.sendMessage(oss.str());
// }
