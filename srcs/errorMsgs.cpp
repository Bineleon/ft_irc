#include "../includes/errorMsgs.hpp"

std::map<int, std::pair<ErrorFormat, std::string> > clientMessages;

void	initErrorMessages()
{
	clientMessages[ERR_PASSWDMISMATCH] = std::make_pair(CLIENT, "Password incorrect");
	clientMessages[ERR_NEEDMOREPARAMS] = std::make_pair(CLIENT_CMD, "Not enough parameters");
	clientMessages[ERR_ALREADYREGISTERED] = std::make_pair(CLIENT, "You may not reregister");
	clientMessages[ERR_NONICKNAMEGIVEN] = std::make_pair(CLIENT, "No nickname given");
	clientMessages[ERR_ERRONEUSNICKNAME] = std::make_pair(CLIENT_NICK, "Erroneus nickname"); // invalid characters in the nickname
	clientMessages[ERR_NICKNAMEINUSE] = std::make_pair(CLIENT_NICK, "Nickname is already in use");
	clientMessages[ERR_NOSUCHCHANNEL] = std::make_pair(CLIENT_CHANNEL, "No such channel");

	// clientMessages[ERR_ALREADYREGISTERED] = std::make_pair(NO_ARG, "You may not reregister");
	// clientMessages[ERR_PASSWDMISMATCH] = std::make_pair(NO_ARG, "Password incorrect");
	// clientMessages[ERR_NEEDMOREPARAMS] = std::make_pair(NO_ARG, "Not enough parameters");
	// clientMessages[ERR_NOTREGISTERED]  = std::make_pair(NO_ARG, "You have not registered");
	// clientMessages[ERR_NORECIPIENT] = std::make_pair(NO_ARG, "No recipient given");
	// clientMessages[ERR_NOTEXTTOSEND] = std::make_pair(NO_ARG, "No text to send");
	// clientMessages[ERR_NOPRIVILEGES] = std::make_pair(NO_ARG, "Permission Denied - You're not an IRC operator");
	// clientMessages[ERR_USERSDONTMATCH] = std::make_pair(NO_ARG, "Cannot change mode for other users");

	// clientMessages[ERR_NONICKNAMEGIVEN] = std::make_pair(WITH_ARG, "No nickname given");
	// clientMessages[ERR_ERRONEUSNICKNAME] = std::make_pair(WITH_ARG, "Erroneous nickname");
	// clientMessages[ERR_NICKNAMEINUSE] = std::make_pair(WITH_ARG, "Nickname is already in use");
	// clientMessages[ERR_NOSUCHNICK] = std::make_pair(WITH_ARG, "No such nick");
	// clientMessages[ERR_NOSUCHCHANNEL] = std::make_pair(WITH_ARG, "No such channel");
	// clientMessages[ERR_CANNOTSENDTOCHAN] = std::make_pair(WITH_ARG, "Cannot send to channel");
	// clientMessages[ERR_UNKNOWNCOMMAND] = std::make_pair(WITH_ARG, "Unknown command");
	// clientMessages[ERR_NOTONCHANNEL] = std::make_pair(WITH_ARG, "You're not on that channel");
	// clientMessages[ERR_USERNOTINCHANNEL] = std::make_pair(WITH_ARG, "They aren't on that channel");
	// clientMessages[ERR_USERONCHANNEL] = std::make_pair(WITH_ARG, "User is already on channel");
	// clientMessages[ERR_CHANNELISFULL] = std::make_pair(WITH_ARG, "Cannot join channel (+l)");
	// clientMessages[ERR_INVITEONLYCHAN] = std::make_pair(WITH_ARG, "Cannot join channel (+i)");
	// clientMessages[ERR_BANNEDFROMCHAN] = std::make_pair(WITH_ARG, "Cannot join channel (+b)");
	// clientMessages[ERR_BADCHANNELKEY] = std::make_pair(WITH_ARG, "Cannot join channel (+k)");
	// clientMessages[ERR_CHANOPRIVSNEEDED] = std::make_pair(WITH_ARG, "You're not channel operator");

}