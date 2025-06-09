#ifndef ERRORMSGS_HPP
#define ERRORMSGS_HPP

#include "Server.hpp"
#include <map>

// enum ErrorFormat
// {
    // 	NO_ARG,
    // 	WITH_ARG
    // };
    
// enum ErrorFormat {
//     CLIENT,
//     CLIENT_NICK,
//     CLIENT_CMD,
//     CLIENT_CHANNEL
// };
    
// extern std::map<int, std::pair< ErrorFormat, std::string> > clientMessages;

#define ERR_ALREADYREGISTERED     462
#define ERR_PASSWDMISMATCH        464
#define ERR_NEEDMOREPARAMS        461
#define ERR_NOTREGISTERED         451
#define ERR_NORECIPIENT           411
#define ERR_NOTEXTTOSEND          412
#define ERR_NOPRIVILEGES          481
#define ERR_USERSDONTMATCH        502

#define ERR_NONICKNAMEGIVEN       431
#define ERR_ERRONEUSNICKNAME      432
#define ERR_NICKNAMEINUSE         433
#define ERR_NOSUCHNICK            401
#define ERR_NOSUCHCHANNEL         403
#define ERR_CANNOTSENDTOCHAN      404
#define ERR_UNKNOWNCOMMAND        421
#define ERR_NOTONCHANNEL          442
#define ERR_USERNOTINCHANNEL      441
#define ERR_USERONCHANNEL         443
#define ERR_CHANNELISFULL         471
#define ERR_INVITEONLYCHAN        473
#define ERR_BANNEDFROMCHAN        474
#define ERR_BADCHANNELKEY         475
#define ERR_CHANOPRIVSNEEDED      482


#endif
