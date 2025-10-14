#ifndef REPLIES_HPP
#define REPLIES_HPP

// Welcome & Authentication
#define RPL_WELCOME				001  // "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
#define RPL_YOURHOST			002  // "<client> :Your host is <servername>, running version <version>"
#define RPL_CREATED 			003  // "<client> :This server was created <datetime>"
#define RPL_MYINFO				004  // "<client> <servername> <version> <available user modes> <available channel modes> [<channel modes with a parameter>]"

// Join/Names
#define RPL_TOPIC				332  // "<client> <channel> :<topic>"
#define RPL_NOTOPIC				331  // "<client> <channel> :No topic is set"
#define RPL_TOPICWHOTIME		333  // "<client> <channel> <nick> <setat>"
#define RPL_NAMREPLY			353  // "<client> = <channel> :[[@|@]nick *( " " [@|@]nick )]"
#define RPL_ENDOFNAMES			366

// Mode & Channel
#define RPL_CHANNELMODEIS		324  // "<client> <channel> <modestring> <mode arguments>..."
#define RPL_CREATIONTIME		329  // "<client> <channel> <creationtime>"

#define RPL_INVITING			341  // "<client> <nick> <channel>"

// Ping/Pong
#define RPL_PING				0    // PING/PONG

#endif