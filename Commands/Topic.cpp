
#include "Topic.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Topic::Topic(Core::User* Emitter,
                       Core::Server* Context,
                       CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Topic::ValidateInput(void)
{
  if (!raw->HasArguments())
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetChannelFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHCHANNEL(emitter, raw->Argument(0));
  return 0;
}

int
Commands::Topic::Execute(void)
{
  if (!targetChannel->isUser(emitter))
    return Replies::ERR_NOTONCHANNEL(emitter, targetChannel->getName());
  if (raw->HasTrailing()) {
    if (!targetChannel->getIsTopicModifiable() &&
        !targetChannel->isAdmin(emitter))
      return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(0));
    std::string newTopic = raw->Trailing();
    targetChannel->setTopic(newTopic);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " TOPIC #" +
                             targetChannel->getName() + " :" + newTopic);
    return (0);
  }
  if (targetChannel->getTopic().empty())
    return Replies::RPL_NOTOPIC(emitter, raw->Argument(0));
  else
    return Replies::RPL_TOPIC(emitter, targetChannel);
}
