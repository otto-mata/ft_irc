#include "ExecutableCommand.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "CommandParser.hpp"

ExecutableCommand::ExecutableCommand(User* Emitter,
                                     Server* Context,
                                     MessageCommand* Raw)
  : emitter(Emitter)
  , ctx(Context)
  , raw(Raw)
{
  targetUser = 0;
  targetChannel = 0;
}

ExecutableCommand::~ExecutableCommand() {}

void
ExecutableCommand::SetTargetUser(User* Target)
{
  targetUser = Target;
}

void
ExecutableCommand::SetTargetUserFromContext(const std::string& TargetName)
{
  targetUser = ctx->FindUserByNickname(TargetName);
}

void
ExecutableCommand::SetTargetChannel(Channel* Target)
{
  targetChannel = Target;
}

void
ExecutableCommand::SetTargetChannelFromContext(const std::string& TargetName)
{
  targetChannel = ctx->FindChannelByName(TargetName);
}
