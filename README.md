# ft_irc — Minimal IRC Server (C++98)

## Overview
**ft_irc** is a 42 School project that implements a **minimal IRC server** written in C++98.  
It is compatible with the IRC client irssi and supports basic authentication, client and channel management, and a subset of classic IRC commands based on RFC1459/2812.

> Executable : `ircserv`  
> Usage : `./ircserv <port> <password>`  
> Network loop : `poll()` + non-blocking sockets

---

## Main Features
- **Client management** : non-blocking sockets, `poll()`, read/write buffers, clean disconnection.
- **Authentication** : sequence `PASS` → `NICK` → `USER`, transition to `AUTHENTICATED` state, send welcome replies (`RPL_WELCOME`, etc.).
- **Channel management** : dynamic creation/join, operator rights, topics (`TOPIC`), invitations, user limits and keys.
- **Messaging** : `PRIVMSG` to a **user** or a **channel**, broadcast inside channels.
- **Supported channel modes** :
  - `+i` : **invite-only** channel
  - `+t` : **topic protected** (only ops can modify it)
  - `+k <key>` : channel **key/password**
  - `+l <n>` : **user limit**
  - `+o <nick>` : add/remove **operator**
- **Error handling** : proper `ERR_*` codes (e.g., `ERR_NEEDMOREPARAMS`, `ERR_NOSUCHCHANNEL`, `ERR_CHANOPRIVSNEEDED`…).
- **Automatic PONG** : response to client `PING` requests.
- **Readable logs** : `[INFO]`, `[RECV]`, `[SEND]`, `[ERROR]` with colored output.

---

##  Supported Commands (summary)
| Command | Example | Description |
|---|---|---|
| `PASS` | `PASS <password>` | Must be sent **before** NICK/USER to register. |
| `NICK` | `NICK <nickname>` | Sets or changes nickname (updates all joined channels). |
| `USER` | `USER <username> 0 * :Real Name` | Completes authentication and triggers welcome replies. |
| `PRIVMSG` | `PRIVMSG <nick> :hello` / `PRIVMSG #chan :hi` | Private message to a **user** or **channel**. |
| `JOIN` | `JOIN #chan` / `JOIN #chan key` / `JOIN #a,#b keyA,keyB` | Creates or joins a channel (with mode checks). |
| `PART` | `PART #chan [:reason]` | Leaves a channel and broadcasts the PART message. |
| `TOPIC` | `TOPIC #chan :new topic` / `TOPIC #chan` | Sets or displays the topic (restricted if `+t`). |
| `MODE` | `MODE #chan +i` / `MODE #chan +k <key>` / `MODE #chan +l <n>` / `MODE #chan +o <nick>` | Sets channel modes as listed above. |
| `INVITE` | `INVITE <nick> #chan` | Invites a user (required if `+i` set). |
| `KICK` | `KICK #chan <nick> [:reason]` | Kicks a user from a channel (op required). |
| `PING` | `PING :token` | Server responds with `PONG :token`. |
| `QUIT` | `QUIT [:reason]` | Graceful disconnection: broadcasts QUIT and cleans resources. |
| `CAP` | `CAP LS` / `CAP END` | Recognized but ignored (no capability negotiation). |

---

## Project Structure
```
ft_irc_new/
├─ Makefile
├─ includes/
│  ├─ Server.hpp        # Main server class: sockets, poll, client/channel tables, command dispatch
│  ├─ Client.hpp        # Client state (fd, IP, nick/user/realname, buffers, auth status)
│  ├─ Channel.hpp       # Channel management (users, ops, invites, modes +i/+t/+k/+l/+o, topic)
│  ├─ Command.hpp       # Command parsing
│  ├─ replies.hpp       # RPL_* reply codes
│  ├─ errorMsgs.hpp     # ERR_* error codes
│  └─ utils.hpp         # Logging, conversions, signal handling
└─ srcs/
   ├─ main.cpp          # Entry point, argument parsing, main loop
   ├─ Server.cpp        # Socket setup, poll loop, read/write handling
   ├─ accept.cpp        # Accept new clients
   ├─ read.cpp          # Read input, parse commands, dispatch
   ├─ commandExe.cpp    # Implements PASS/NICK/USER/JOIN/PART/PRIVMSG/TOPIC/MODE/INVITE/KICK/QUIT/PONG
   ├─ Channel.cpp       # User/ops management, mode handling, invites, broadcast
   ├─ Client.cpp        # Buffer management, auth status, nick!user@host masks
   ├─ replies.cpp       # Builds RPL_*/ERR_* responses
   ├─ utils.cpp         # Helper functions, logging, signal utils
   ├─ cleanAll.cpp      # Cleans clients/channels
   └─ handleSignals.cpp # Signal traps for shutdown
```

---

## Requirements
- **C++98**
- POSIX environment (Linux/macOS)
- Network headers (`<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`)
- `make`

---

## Build
```bash
make          # build ./ircserv (creates objects/ and .d dependency files)
make clean    # remove objects and dependencies
make fclean   # remove objects + binary
make re       # full rebuild
```

---

## Run
```bash
./ircserv <port> <password>
# Example:
./ircserv 6667 secret
```

---

## Connect with a client
### Using `netcat`
```bash
nc -C 127.0.0.1 6667
PASS secret
NICK nickname
USER nickname 0 * :Nick Name
JOIN #chan
PRIVMSG #chan :hello everyone
```

### Using `irssi`
```bash
irssi -c localhost -p 6667
# /set nick nickname
# /server add localhost 127.0.0.1 6667 secret
# /connect localhost
# /join #chan
# /msg #chan hello
```

---

## Quick Test Scenarios
- **Auth** : skip `PASS` → receive `ERR_PASSWDMISMATCH` or `ERR_NOTREGISTERED`.
- **PRIVMSG user** : send to non-existing nick → `ERR_NOSUCHNICK`.
- **PRIVMSG #chan** : send to a channel you’re not in → `ERR_CANNOTSENDTOCHAN` or `ERR_NOSUCHCHANNEL`.
- **JOIN** : bad key → `ERR_BADCHANNELKEY`; full → `ERR_CHANNELISFULL`; invite-only → `ERR_INVITEONLYCHAN`.
- **MODE** : `MODE #chan +o <nick>` adds an operator; `MODE #chan +l 10` sets limit.
- **KICK** : non-op tries → `ERR_CHANOPRIVSNEEDED`.
- **PING/PONG** : `PING :xyz` → server replies `PONG :xyz`.
- **QUIT** : clean removal from all channels, broadcast `QUIT`.

---

## Implementation Details
- **Event loop** : single `poll()` loop, `POLLOUT` enabled only when pending writes exist.
- **Non-blocking** : all client sockets set to `O_NONBLOCK`.
- **Memory management** : clean removal from `clients`, `nickClients`, `Channel` lists, and destruction of empty channels.
- **Nickname propagation** : updates all maps (`users`, `operators`, `invited`) on NICK change.
- **Logging** : helper functions (`logInfo`, `logRecv`, `logSend`, `logError`) with colors.
- **Signal handling** : catch signals for safe shutdown.

---

## 👤 Author

**Nelbi Leon**  
42 Paris | [GitHub](https://github.com/Bineleon)


