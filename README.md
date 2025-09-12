# ft_irc

A complete IRC (Internet Relay Chat) server implementation in C++98, compatible with official IRC clients and following RFC 1459 specifications.

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Commands](#commands)
- [Data Structures](#data-structures)
- [Build System](#build-system)
- [Development](#development)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Full IRC Protocol Support**: Implements RFC 1459 compliant IRC server
- **Multi-client Support**: Handles multiple concurrent connections using `select()`
- **Non-blocking I/O**: Efficient handling of client communications
- **Channel Management**: Complete channel system with operators, modes, and permissions
- **User Authentication**: Password-protected server access
- **Comprehensive Command Set**: All major IRC commands implemented
- **Message of the Day (MOTD)**: Customizable welcome message
- **Logging System**: Detailed logging with different log levels
- **Error Handling**: Robust error handling and validation
- **Memory Management**: Proper resource cleanup and memory management

## Architecture

### Core Components

The server is built around several key components:

#### Server Class (`Core::Server`)
- Manages the main server socket and client connections
- Handles incoming connections and data reception
- Manages user and channel collections
- Provides broadcasting capabilities
- Implements the main server loop using `select()`

#### User Class (`Core::User`)
- Represents connected clients
- Manages user state (nickname, username, realname, etc.)
- Handles input/output buffers
- Tracks registration status and capabilities

#### Channel Class (`Core::Channel`)
- Represents IRC channels
- Manages channel users, operators, and permissions
- Handles channel modes and settings
- Provides broadcasting to channel members

#### Command System
- Modular command implementation
- Each command inherits from `ExecutableCommand`
- Input validation and execution phases
- Error handling with appropriate IRC replies

### Network Architecture

- **Socket Server**: Uses TCP sockets with `bind()`, `listen()`, and `accept()`
- **Multi-client Handling**: Uses `select()` for efficient I/O multiplexing
- **Non-blocking Operations**: All I/O operations are non-blocking
- **Buffer Management**: Separate buffers for incoming and outgoing data per client
- **Partial Message Handling**: Buffers incomplete messages until `\r\n` termination

## Requirements

- **C++ Compiler**: C++98 compliant compiler (tested with g++)
- **Operating System**: Linux/Unix-like systems
- **Build Tools**: Make
- **Libraries**: Standard C++ library only (no external dependencies)

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/otto-mata/ft_irc.git
   cd ft_irc
   ```

2. **Build the project**:
   ```bash
   make
   ```

   Optional build flags:
   - `make DEBUG=1`: Build with debug symbols
   - `make FSAN=1`: Build with address sanitizer

3. **Clean build artifacts**:
   ```bash
   make clean    # Remove object files
   make fclean   # Remove object files and executable
   make re       # Rebuild from scratch
   ```

## Usage

### Starting the Server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `<port>`: Port number to listen on (1-65535)
- `<password>`: Server password for client authentication

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connecting with IRC Clients

Use any RFC 1459 compliant IRC client:

```bash
# Using netcat for testing
echo "PASS mypassword" | nc localhost 6667
echo "NICK testuser" | nc localhost 6667
echo "USER testuser 0 * :Test User" | nc localhost 6667

# Using irssi
irssi -c localhost -p 6667

# Using weechat
weechat -s localhost -p 6667
```

### Server Control

- **Start**: Run `./ircserv <port> <password>`
- **Stop**: Press `Ctrl+C` to gracefully shutdown
- **Signals**: 
  - `SIGINT` (`Ctrl+C`): Graceful shutdown
  - `SIGQUIT`: Ignored

## Configuration

### Command Line Arguments

The server accepts exactly two arguments:
1. **Port**: TCP port to bind to (must be available)
2. **Password**: Server password (required for client connections)

### Message of the Day (MOTD)

The server displays a welcome message from `.motd` file in the root directory. Edit this file to customize the MOTD.

### Build Configuration

Modify the `Makefile` for custom build settings:
- `CXXFLAGS`: Compiler flags
- `CXX`: Compiler selection
- `DEBUGF`: Debug flags
- `FSANF`: Sanitizer flags

## Commands

The server implements the following IRC commands:

### Connection Commands
- `PASS <password>`: Authenticate with server password
- `NICK <nickname>`: Set user nickname
- `USER <username> <hostname> <servername> :<realname>`: Register user information
- `QUIT [:<message>]`: Disconnect from server

### Channel Commands
- `JOIN <channel> [<password>]`: Join a channel
- `PART <channel> [:<message>]`: Leave a channel
- `TOPIC <channel> [:<topic>]`: View or set channel topic
- `LIST [<channel>]`: List channels and their topics

### Communication Commands
- `PRIVMSG <target> <message>`: Send private message
- `NOTICE <target> <message>`: Send notice message

### Channel Management
- `MODE <channel> <mode> [<parameters>]`: Change channel modes
- `INVITE <nickname> <channel>`: Invite user to channel
- `KICK <channel> <user> [<comment>]`: Remove user from channel

### Server Commands
- `PING <server>`: Test connection
- `PONG <server>`: Respond to ping
- `WHOIS <nickname>`: Get user information
- `WHOWAS <nickname>`: Get information about disconnected users

### Advanced Features
- `CAP <subcommand> [<capabilities>]`: Capabilities negotiation

## Data Structures

### User Management
```cpp
class User {
    int _fd;                    // Socket file descriptor
    std::string _nickname;      // User nickname
    std::string _username;      // User username
    std::string _realname;      // Real name
    std::string _hostname;      // Client hostname
    std::string _ipAdress;      // IP address
    std::string _incomingBuffer; // Input buffer
    std::string _outgoingBuffer; // Output buffer
    bool _hasNick;              // Nickname set
    bool _hasUser;              // Username set
    bool _hasSentPassword;      // Password sent
    bool _isValidPassword;      // Password valid
    bool _disconnected;         // Disconnection flag
}
```

### Channel Management
```cpp
class Channel {
    std::string _name;          // Channel name
    Users _users;               // Channel users
    Users _whitelist;           // Invited users
    Users _admins;              // Channel operators
    User* _owner;               // Channel owner
    std::string _topic;         // Channel topic
    std::string _password;      // Channel password
    size_t _userLimit;          // User limit
    bool _isInviteOnly;         // Invite-only mode
    bool _isTopicModifiable;    // Topic protection
    bool _isUserLimited;        // User limit active
    bool _isPasswordProtected;  // Password protection
}
```

### Server State
```cpp
class Server {
    unsigned short port;        // Listening port
    int fd;                     // Server socket
    std::string password;       // Server password
    UserMap users;              // Connected users
    ChannelMap channels;        // Active channels
    BufferMap buffers;          // Client buffers
    std::string hostName;       // Server hostname
}
```

## Build System

### Makefile Structure

The build system uses GNU Make with the following features:

- **Automatic Dependency Generation**: Uses `-MMD -MP` flags
- **Object File Management**: Organized in `build/` directory
- **Modular Compilation**: Separate compilation of source files
- **Build Variants**: Debug and sanitizer builds

### Source Organization

```
src/
├── main.cpp                    # Entry point
├── Commands/                   # IRC commands
├── Core/                       # Core server logic
│   ├── Args/                   # Argument parsing
│   ├── Channel/                # Channel management
│   ├── Server/                 # Server implementation
│   └── User/                   # User management
├── Common/                     # Shared utilities
├── Logging/                    # Logging system
├── Parsing/                    # Message parsing
└── Replies/                    # IRC reply generation
```

## Development

### Code Style

- **Language**: C++98 standard
- **Naming**: Snake_case for variables and functions
- **Classes**: PascalCase for class names
- **Namespaces**: Organized under `Core` and `Commands`
- **Error Handling**: Return codes and exceptions
- **Memory Management**: RAII principles

### Adding New Commands

1. Create command class in `src/Commands/`
2. Inherit from `ExecutableCommand`
3. Implement `ValidateInput()` and `Execute()`
4. Add to command factory/registry
5. Update `Commands.hpp`

### Extending Server Features

- **New Channel Modes**: Modify `Channel` class
- **Authentication Methods**: Extend `User` registration
- **Network Protocols**: Add to server socket handling
- **Logging Levels**: Enhance `Logging::Engine`

## Testing

### Manual Testing

1. **Start Server**:
   ```bash
   ./ircserv 6667 testpass
   ```

2. **Connect Multiple Clients**:
   ```bash
   # Terminal 1
   nc localhost 6667 -C
   
   # Terminal 2
   nc localhost 6667 -C
   ```

3. **Test Commands**:
   ```
   PASS testpass
   NICK user1
   USER user1 0 * :Test User 1
   JOIN #test
   PRIVMSG #test Hello World!
   ```

### Automated Testing

The project includes comprehensive error checking and validation. Test scenarios include:

- **Connection Limits**: Multiple concurrent connections
- **Command Validation**: Invalid parameters and sequences
- **Channel Operations**: Mode changes, user management
- **Error Conditions**: Network failures, invalid inputs

## License

This project is part of the 42 School curriculum. See project requirements for usage terms.

---

**Note**: This implementation is designed for educational purposes and follows the IRC RFC 1459 specification. For production use, consider additional security measures and scalability improvements.
