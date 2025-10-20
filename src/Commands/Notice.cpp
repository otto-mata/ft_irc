#include "Notice.hpp"
#include <Channel.hpp>
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Notice::Notice(Core::User *Emitter,
                           Core::Server *Context,
                           CommandParser::MessageCommand *Raw)
    : ExecutableCommand(Emitter, Context, Raw) {
}

int
Commands::Notice::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments() || !raw->HasTrailing())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetChannelFromContext(raw->Argument(0)))
  {
    if (!SetTargetUserFromContext(raw->Argument(0)))
      return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  }
  return 0;
}

int
Commands::Notice::Execute(void)
{
  std::string base = ":" + emitter->FullIdentityString() + " NOTICE " + raw->Trailing();
  if (targetChannel)
    targetChannel->Broadcast(base, emitter);
  else if (targetUser)
    targetUser->AppendToOutgoingBuffer(base);
  return 0;
}

