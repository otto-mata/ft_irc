
#include "Privmsg.hpp"
#include <Channel.hpp>
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
  if (raw->Argument(0).at(0) == '#') {
    if (!SetTargetChannelFromContext(raw->Argument(0)))
      return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(0));
	if (!targetChannel->IsUser(emitter))
	  return Replies::ERR_NOTONCHANNEL(emitter, raw->Argument(0));
  } else if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  return 0;
}

int
Commands::Privmsg::Execute(void)
{
  if (targetChannel && targetChannel->IsUser(emitter)) {
    targetChannel->Broadcast(":" + emitter->FullIdentityString() +
                               " PRIVMSG #" + targetChannel->GetName() + " :" +
                               raw->Trailing(),
                             emitter);
  if (targetUser)
    targetUser->AppendToOutgoingBuffer(":" + emitter->FullIdentityString() +
                                       " PRIVMSG " + targetUser->GetNickname() +
                                       " :" + raw->Trailing());
  return 0;
}
