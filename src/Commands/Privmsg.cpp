#include "Privmsg.hpp"
#include <Channel.hpp>
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Privmsg::Privmsg(Core::User *Emitter,
                           Core::Server *Context,
                           CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
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
      if (!SetTargetChannelFromContext(*it)) {
        Replies::ERR_NOSUCHCHANNEL(emitter, *it);
        continue;
      }
      if (!targetChannel->IsUser(emitter)) {
        Replies::ERR_NOTONCHANNEL(emitter, *it);
        continue;
      }
      if (targetChannel && targetChannel->IsUser(emitter)){
        targetChannel->Broadcast(base + "#" + targetChannel->GetName() + " :" +
                                raw->Trailing(), emitter);
        if (targets.size() > 1)
          emitter->AppendToOutgoingBuffer(base + "#" + targetChannel->GetName() + " :" +
                                  raw->Trailing());
      }
    }
    else {
      if (!SetTargetUserFromContext(*it) || targetUser == emitter) {
        Replies::ERR_NOSUCHNICK(emitter, *it);
        continue;
      }
      if (targetUser) {
        targetUser->AppendToOutgoingBuffer(base + targetUser->GetNickname() + " :" + 
                                            raw->Trailing());
        if (targets.size() > 1)
          emitter->AppendToOutgoingBuffer(base + targetUser->GetNickname() + " :" + 
                                            raw->Trailing());
      }
    }
  }
  return 0;
}

