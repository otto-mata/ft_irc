#ifndef EXECUTABLE_COMMAND_HPP
#define EXECUTABLE_COMMAND_HPP
#include <string>

class User;
class Channel;
class Server;
class MessageCommand;

class ExecutableCommand
{
private:
  User* emitter;
  User* targetUser;
  Channel* targetChannel;
  Server* ctx;
  MessageCommand* raw;

public:
  ExecutableCommand(User* Emitter, Server* Context, MessageCommand* raw);
  ~ExecutableCommand();
  
  virtual int ValidateInput() = 0;
  virtual int Execute() = 0;

  void SetTargetUser(User* Target);
  void SetTargetUserFromContext(const std::string& TargetName);
  void SetTargetChannel(Channel* Target);
  void SetTargetChannelFromContext(const std::string& TargetName);

};

#endif
