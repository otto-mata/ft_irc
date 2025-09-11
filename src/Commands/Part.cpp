
#include "Part.hpp"
#include <Channel.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Part::Part(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Part::ValidateInput(void)
{
  if (!raw->HasArguments()) 
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (raw->Argument(1).find('#') != 0 ||
      !SetTargetChannelFromContext(raw->Argument(1)))
    return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(1));
  if (!targetChannel->IsUser(emitter))
    return Replies::ERR_NOTONCHANNEL(emitter, targetChannel->GetName());
  return 0;
}

int
Commands::Part::Execute(void)
{

  std::string broadcast = ":" + emitter->FullIdentityString() + " " +
                          emitter->GetNickname() + " PART #" +
                          targetChannel->GetName();
  if (raw->HasTrailing())
    broadcast += " :" + raw->Trailing();
  targetChannel->Broadcast(broadcast);
  targetChannel->RemoveUser(targetUser);
  targetChannel->RemoveAdmin(targetUser);
  if (targetChannel->GetUsers().empty() && targetChannel->IsInviteOnly())
    ctx->RemoveChannel(targetChannel);
  return 0;
}
