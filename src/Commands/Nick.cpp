
#include "Nick.hpp"
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Nick::Nick(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}
int
Commands::Nick::ValidateInput(void)
{
  if (ctx->IsPasswordProtected() && !emitter->HasSentValidPassword())
    return 1;
  if (!raw->HasArguments()) {
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  }
  if (ctx->MatchUserByNickname(raw->Argument(0))) {
    return Replies::ERR_NICKNAMEINUSE(emitter, raw->Argument(0));
  }
  return 0;
}

int
Commands::Nick::Execute(void)
{
  const std::string& name = raw->Argument(0);
  if (emitter->FullyRegistered()) {
    Core::ChannelMap  joined = emitter->getJoinedChanels();
    std::string       broadcast(":" + emitter->FullIdentityString() + " NICK :" + name);

    if (joined.size() == 0)
      emitter->AppendToOutgoingBuffer(broadcast);
    else
      for (Core::ChannelMap::iterator it = joined.begin() ; it != joined.end() ; it++)
        it->second->Broadcast(broadcast);
  }
  emitter->SetNickname(name);
  emitter->CompletedRegistrationRoutine(ctx->Hostname());
  ctx->LogNicknameChangeForUser(emitter);
  return 0;
}
