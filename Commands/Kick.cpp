
#include "Kick.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Kick::Kick(User* Emitter,
                     Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Kick::ValidateInput(void)
{
  if (raw->Arguments().size() < 2)
    return 1; //! Invalid arguments count (not enough)
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return 2; //! Target user does not exist
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).find('#') != 0)
    return 4; //! Invalid target channel name (must start with '#')
  if (!SetTargetChannelFromContext(raw->Argument(1)))
    return 5; //! Target channel does not exist
  if (!targetChannel->isAdmin(emitter))
    return 6; //! emitter not operator in channel
  if (!targetChannel->isUser(targetUser))
    return 7; //! Target user not in channel
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
  if (raw->Trailing().size() > 0)
    broadcast += " :" + raw->Trailing();
  targetChannel->Broadcast(broadcast);
  targetChannel->removeUser(targetUser);
  targetChannel->removeAdmin(targetUser);
  if (targetChannel->GetUsers().empty())
    ctx->RemoveChannel(targetChannel);
  return 0;
}
