#include "ExecutableCommand.hpp"
#include "Channel.hpp"
#include "CommandParser.hpp"
#include "Server.hpp"
#include "User.hpp"

ExecutableCommand::ExecutableCommand(User* Emitter,
                                     Server* Context,
                                     CommandParser::MessageCommand* Raw)
  : emitter(Emitter)
  , ctx(Context)
  , raw(Raw)
{
  targetUser = 0;
  targetChannel = 0;
}

ExecutableCommand::~ExecutableCommand() {}

bool
ExecutableCommand::SetTargetUser(User* Target)
{
  targetUser = Target;
  return targetUser != 0;
}

bool
ExecutableCommand::SetTargetUserFromContext(const std::string& TargetName)
{
  targetUser = ctx->FindUserByNickname(TargetName);
  return targetUser != 0;
}

bool
ExecutableCommand::SetTargetChannel(Channel* Target)
{
  targetChannel = Target;
  return targetChannel != 0;
}

bool
ExecutableCommand::SetTargetChannelFromContext(const std::string& TargetName)
{
  size_t pos = 0;
  if (TargetName.find('#') == 0)
    pos++;
  targetChannel = ctx->FindChannelByName(TargetName.substr(pos));
  return targetChannel != 0;
}
