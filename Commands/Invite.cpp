
#include "Invite.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"
#include "../Replies/Replies.hpp"

Commands::Invite::Invite(Core::User* Emitter,
                         Core::Server* Context,
                         CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Invite::ValidateInput(void)
{
  if (!raw->HasArguments() || raw->Arguments().size() < 2) 
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(0));
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).at(0) != '#')
    return 4; //! Invalid target channel name (must start with '#')
  if (!SetTargetChannelFromContext(raw->Argument(1)))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(1));
  if (!targetChannel->isAdmin(emitter))
    return 6; //! Emitter is not an operator of the channel
  if (targetChannel->isUser(targetUser))
    return 7; //! Target user is already in the channel
  return 0;
}

int
Commands::Invite::Execute(void)
{
  targetChannel->addUserWhitelist(targetUser);
  targetUser->AppendToOutgoingBuffer(":" + emitter->GetNickname() + " INVITE " +
                                     targetUser->GetNickname() + " " +
                                     targetChannel->getName());
  emitter->AppendToOutgoingBuffer(":localhost 341 " + emitter->GetNickname() +
                                  " " + targetUser->GetNickname() + " " +
                                  targetChannel->getName());
  return 0;
}
