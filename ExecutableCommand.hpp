#ifndef EXECUTABLE_COMMAND_HPP
#define EXECUTABLE_COMMAND_HPP
#include "CommandParser.hpp"
#include <string>

class User;
class Channel;
class Server;

namespace Commands {
typedef enum
{
  VALID_ARGS = 0
};
} // namespace Commands

class ExecutableCommand
{
protected:
  User* emitter;
  User* targetUser;
  Channel* targetChannel;
  Server* ctx;
  CommandParser::MessageCommand* raw;

public:
  ExecutableCommand(User* Emitter,
                    Server* Context,
                    CommandParser::MessageCommand* raw);
  ~ExecutableCommand();

  virtual int ValidateInput() = 0;
  virtual int Execute() = 0;

  bool SetTargetUser(User* Target);
  bool SetTargetUserFromContext(const std::string& TargetName);
  bool SetTargetChannel(Channel* Target);
  bool SetTargetChannelFromContext(const std::string& TargetName);
};

#endif
