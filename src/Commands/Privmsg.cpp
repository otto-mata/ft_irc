
#include "Privmsg.hpp"
#include <Channel.hpp>
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Privmsg::Privmsg(Core::User* Emitter,
                           Core::Server* Context,
                           CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Privmsg::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments() || !raw->HasTrailing())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  return 0;
}

int
Commands::Privmsg::Execute(void)
{
  std::string base = ":" + emitter->FullIdentityString() + " PRIVMSG ";
  std::vector<std::string> targets =
    Algo::String::Split(std::string(raw->Argument(0)), ",");
  
  for (std::vector<std::string>::iterator it = targets.begin();
      it != targets.end();
      it++) {
    if (it->at(0) == '#') {
      if (!SetTargetChannelFromContext(raw->Argument(0))) {
        Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(0));
        continue;
      }
      if (!targetChannel->IsUser(emitter)) {
        Replies::ERR_NOTONCHANNEL(emitter, raw->Argument(0));
        continue;
      }
      if (targetChannel && targetChannel->IsUser(emitter))
        targetChannel->Broadcast( base + "#" + targetChannel->GetName() + " :" +
                                raw->Trailing(), emitter);
    }
    else {
      if (!SetTargetUserFromContext(raw->Argument(0)) || targetUser == emitter) {
        Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
        continue;
      }
      if (targetUser)
        targetUser->AppendToOutgoingBuffer(base + targetUser->GetNickname() + " :" + 
                                            raw->Trailing());
      }
    }
  return 0;
}

