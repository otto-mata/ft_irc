
#include "Invite.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Invite::Invite(User* Emitter,
                         Server* Context,
                         CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Invite::ValidateInput(void)
{
  if (raw->Arguments().size() < 2)
    return 1;
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return 2; //! Target user does not exist
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).find('#') != 0)
    return 4; //! Invalid target channel name (must start with '#')
  if (!SetTargetChannelFromContext(raw->Argument(1)))
    return 5; //! Target channel does not exist
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
