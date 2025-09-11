#ifndef EXECUTABLE_COMMAND_HPP
#define EXECUTABLE_COMMAND_HPP
#include "CommandParser.hpp"
#include <string>
#include <iostream>
#define TRACE_CALL  std::cout << __PRETTY_FUNCTION__ <<" called." << std::endl;
namespace Core {

class User;
class Channel;
class Server;

} // namespace Core

namespace Commands {
typedef enum
{
  VALID_ARGS = 0
} ReturnCodes;
} // namespace Commands

class ExecutableCommand
{
protected:
  Core::User* emitter;
  Core::User* targetUser;
  Core::Channel* targetChannel;
  Core::Server* ctx;
  CommandParser::MessageCommand* raw;

public:
  ExecutableCommand(Core::User* Emitter,
                    Core::Server* Context,
                    CommandParser::MessageCommand* raw);
  virtual ~ExecutableCommand();

  virtual int ValidateInput() = 0;
  virtual int Execute() = 0;

  bool SetTargetUser(Core::User* Target);
  bool SetTargetUserFromContext(const std::string& TargetName, int mode = 0);
  bool SetTargetChannel(Core::Channel* Target);
  bool SetTargetChannelFromContext(const std::string& TargetName);
};

#endif
