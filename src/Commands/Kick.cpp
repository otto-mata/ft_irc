
#include "Kick.hpp"
#include <Channel.hpp>
#include <ExecutableCommand.hpp>
#include <Server.hpp>
#include <User.hpp>
#include <Replies.hpp>

Commands::Kick::Kick(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Kick::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments() || raw->Arguments().size() < 2) {
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  }
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).find('#') != 0 || !SetTargetChannelFromContext(raw->Argument(1)))
    return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(1));
  if (!targetChannel->IsAdmin(emitter))
    return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(1));
  if (!targetChannel->IsUser(targetUser))
    return Replies::ERR_USERONCHANNEL(emitter, targetUser->GetNickname(), targetChannel->GetName());
  return 0;
}

int
Commands::Kick::Execute(void)
{
  targetUser->AppendToOutgoingBuffer(":" + targetUser->GetNickname() +
                                     " PART #" + targetChannel->GetName() +
                                     "\r\n");
  std::string broadcast = ":" + emitter->GetNickname() + " KICK #" +
                          targetChannel->GetName() + " " +
                          targetUser->GetNickname();
  if (raw->HasTrailing())
    broadcast += " :" + raw->Trailing();
  targetChannel->Broadcast(broadcast);
  targetChannel->RemoveUser(targetUser);
  targetChannel->RemoveAdmin(targetUser);
  if (targetChannel->GetUsers().empty() && targetChannel->IsInviteOnly())
    ctx->RemoveChannel(targetChannel);
  return 0;
}
