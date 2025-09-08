# ft_irc

IRC server, compatible with official clients. (See [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459))

## Architecture and work

### Socket server

Create listening socket (bind, listen, accept).

Handle multiple clients using select().

Manage non-blocking I/O.

Handle partial data with packet buffering.
This will be done with a separate `map` , linking an `int fd` to a buffer.
This is critical, as commands should be parsed only upon full reception (indicated by a `\r\n` (`CRLF`)).

The users should be stored in a `map` , linking an `int fd`  to a `User` object.

The channels should be stored in a `map` , linking a `std::string name`  to a `Channel` object.

The server should also be able to broadcast a message to every connected client, and to every connected client except the origin. 

### Channel Object

The `Channel` Object represents a channel created on the server.
It should have the following attributes:

- `std::string name;`
- `User *owner;`
- `std::string password;`
- `size_t maxUsers;`
- `bool inviteOnly;`
- `bool protectedTopic;`
- `std::string topic;`
- `std::list<User *> operators;`
- `std::list<User *> users;`
- `std::list<User *> guests`
- `bool isABotChannel`

### User Object

The `User` Object represents a connected client to the server, whether it is authenticated or not.
It should have the following attributes:

- File Descriptor
- Nickname
- Host name
- Incoming Buffer
- Outgoing Buffer
- Username
- Real Name
- IP Address
- Has Nickname
- Has Username
- Is Password Valid
- Is Registered

### Command Parsing

The command parsing class may be implemented as such:

```cpp
class CommandParser
{
private:
  std::string* source;
  std::string cmd;
  std::vector<std::string>* params;
  std::string* trailing;
public:
  CommandParser(std::string commandStr);
  ~CommandParser();
};
```

This class must parse a full user request into a usable and modular object, simplifying the work of command execution down the line.

### Command Object

Those commands have to be implemented:

- `CAP`
- `INVITE`
- `JOIN`
- `KICK`
- `LIST`
- `MODE`
- `NICK`
- `NOTICE`
- `PART`
- `PASS`
- `PING`
- `PONG`
- `PRIVMSG`
- `QUIT`
- `TOPIC`
- `WHOIS`
- `WHOWAS`

The implementation should be as following:

A main, generic, abstract `CommandBase` class, of which will derive every other command.
This class will have a similar declaration to:

```cpp

class CommandBase
{
protected:
  User* emitter;
  User* targetUser;
  Channel* targetChannel;
  Server* ctx;
  Command* params;
public:
  CommandBase(User* user = 0, Server* server = 0, Command* cmd = 0);
  ~CommandBase();
  virtual int execute() = 0;
  virtual int check() = 0;
};
```

This will ensure every command can be used in the same exact way.

For example, the `NICK` command may be implemented as:

```cpp
class CMDNick
{
public:
	int execute()
	{
		std::string nickname = params->params[0]; // Assuming NICK nickname
		ctx.UpdateUserNickname(emitter, nickname);
	}

	int check()
	{
		return params && params->params.size() > 0;
	}
	
};
```
