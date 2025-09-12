
#include "Topic.hpp"
#include <Channel.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Topic::Topic(Core::User* Emitter,
                       Core::Server* Context,
                       CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Topic::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetChannelFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(0));
  return 0;
}

int
Commands::Topic::Execute(void)
{
  if (!targetChannel->IsUser(emitter))
    return Replies::ERR_NOTONCHANNEL(emitter, targetChannel->GetName());
  if (raw->HasTrailing()) {
    if (!targetChannel->IsTopicModifiable() &&
        !targetChannel->IsAdmin(emitter))
      return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(0));
    std::string newTopic = raw->Trailing();
    targetChannel->SetTopic(newTopic);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " TOPIC #" +
                             targetChannel->GetName() + " :" + newTopic);
    return (0);
  }
  if (targetChannel->GetTopic().empty())
    return Replies::RPL_NOTOPIC(emitter, raw->Argument(0));
  else
    return Replies::RPL_TOPIC(emitter, targetChannel);
}
