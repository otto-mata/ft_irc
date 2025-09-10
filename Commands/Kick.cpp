
#include "Kick.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"
#include "../Replies/Replies.hpp"

Commands::Kick::Kick(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Kick::ValidateInput(void)
{
  if (!raw->HasArguments() || raw->Arguments().size() < 2) {
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  }
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(0));
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).find('#') != 0)
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(1));
  if (!SetTargetChannelFromContext(raw->Argument(1)))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(1));
  if (!targetChannel->isAdmin(emitter))
    return 6; //! emitter not operator in channel
  if (!targetChannel->isUser(targetUser))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(0));
  return 0;
}

int
Commands::Kick::Execute(void)
{
  targetUser->AppendToOutgoingBuffer(":" + targetUser->GetNickname() +
                                     " PART #" + targetChannel->getName() +
                                     "\r\n");
  std::string broadcast = ":" + emitter->GetNickname() + " KICK #" +
                          targetChannel->getName() + " " +
                          targetUser->GetNickname();
  if (raw->HasTrailing())
    broadcast += " :" + raw->Trailing();
  targetChannel->Broadcast(broadcast);
  targetChannel->removeUser(targetUser);
  targetChannel->removeAdmin(targetUser);
  if (targetChannel->GetUsers().empty() && targetChannel->getIsInviteOnly())
    ctx->RemoveChannel(targetChannel);
  return 0;
}
