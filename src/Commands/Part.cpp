#include "Part.hpp"
#include <CustomAlgo.hpp>
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
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  return 0;
}

int
Commands::Part::Execute(void)
{
  std::string broadcast;
  std::vector<std::string> channels =
    Algo::String::Split(std::string(raw->Argument(0)), ",");
  for (std::vector<std::string>::iterator it = channels.begin();
       it != channels.end();
       it++) {  
    
    if (it->at(0) == '#') {
      if (!SetTargetChannelFromContext(*it)) {
        Replies::ERR_NOSUCHCHANNEL(emitter, *it);
        continue;
      }
    }
    else {
      Replies::ERR_NOSUCHCHANNEL(emitter, *it);
      continue;
    }
    if (!targetChannel->IsUser(emitter)) {
      Replies::ERR_NOTONCHANNEL(emitter, targetChannel->GetName());
      continue;
    }
  
    broadcast = ":" + emitter->FullIdentityString() + " PART #" + targetChannel->GetName();;
    if (raw->HasTrailing())
      broadcast += " :" + raw->Trailing();

    targetChannel->Broadcast(broadcast, emitter);
    emitter->AppendToOutgoingBuffer(
      ":IRC!admin@localhost KICK #" + targetChannel->GetName() + " " + emitter->GetNickname());

    targetChannel->RemoveUser(emitter);
    targetChannel->RemoveAdmin(emitter);
    if (targetChannel->GetUsers().empty())
      ctx->RemoveChannel(targetChannel);
  }
  return 0;
}
