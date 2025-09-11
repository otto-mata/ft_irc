
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
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  if (!targetUser->FullyRegistered())
    return 3; //! Target user is not registered to the server
  if (raw->Argument(1).at(0) != '#' || !SetTargetChannelFromContext(raw->Argument(1)))
    return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(1));
  if (!targetChannel->IsAdmin(emitter))
    return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(1));
  if (targetChannel->IsUser(targetUser))
    return Replies::ERR_USERONCHANNEL(emitter, targetUser->GetNickname(), raw->Argument(1));
  return 0;
}

int
Commands::Invite::Execute(void)
{
  targetChannel->AddUserToWhitelist(targetUser);
  targetUser->AppendToOutgoingBuffer(":" + emitter->GetNickname() + " INVITE " +
                                     targetUser->GetNickname() + " " +
                                     targetChannel->GetName());
  emitter->AppendToOutgoingBuffer(":localhost 341 " + emitter->GetNickname() +
                                  " " + targetUser->GetNickname() + " " +
                                  targetChannel->GetName());
  return 0;
}
