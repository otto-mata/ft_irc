
#include "Mode.hpp"
#include "../CustomAlgo.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Mode::Mode(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Mode::ValidateInput(void)
{
  if (!raw->HasArguments()) {
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  }
  return 0;
}

int
Commands::Mode::handleOpMode(char mode)
{
  if (raw->Arguments().size() != 3)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetUserFromContext(raw->Argument(2)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(2));
  if (!targetChannel->isUser(targetUser))
    return Replies::ERR_NOSUCHNICK(emitter, targetUser->GetNickname());
    if (mode == '-')
  {
    if (!targetChannel->isAdmin(targetUser))
      return 1;
    targetChannel->removeAdmin(targetUser);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE -o " + targetUser->GetNickname());
  }
  if (mode == '+')
  {
    if (targetChannel->isAdmin(targetUser))
      return 2;
    targetChannel->addAdmin(targetUser);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE +o " + targetUser->GetNickname());
  }
  return (0);
}

int
Commands::Mode::handleKeyMode(char mode)
{
  if (mode == '+' && targetChannel->getIsPasswordProtected())
    return Replies::ERR_KEYSET(emitter, targetChannel->getName());
  if (mode == '+' && raw->Arguments().size() < 3)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (mode == '+'){
    targetChannel->setPassword(raw->Argument(2));
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE +k " + raw->Argument(2));
  } else if (mode == '-'){
    targetChannel->setPassword("");
    targetChannel->setIsPasswordProtected(false);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE -k ");
  }
  return (0);
}

int
Commands::Mode::handleLimMode(char mode)
{
  if (mode == '+' && raw->Arguments().size() < 3)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (mode == '+'){
    size_t n = 0;
    if (!Algo::String::SaferStoul(raw->Argument(2), &n))
      return 1;
    targetChannel->setUserLimit(n);
  } else if (mode == '-'){
    targetChannel->setIsUserLimited(false);
    targetChannel->setUserLimit(~0);
  }
  return (0);
}

int
Commands::Mode::Execute(void)
{
  if (!SetTargetChannelFromContext(raw->Argument(0))) {
    if (raw->Argument(0) == emitter->GetNickname()) {
      if (raw->Arguments().size() == 1)
        return Replies::RPL_UMODEIS(emitter);
      else
        return Replies::ERR_UMODEUNKNOWNFLAG(emitter, raw->Argument(1));
    }
    return Replies::ERR_USERSDONTMATCH(emitter);
  }
  if (raw->Arguments().size() < 2)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!targetChannel->isUser(emitter))
    return Replies::ERR_NOTONCHANNEL(emitter, raw->Argument(0));
  if (!targetChannel->isAdmin(emitter))
    return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(0));

  std::string flag = raw->Argument(1);
  char mode = flag.at(0);
  if (mode != '+' && mode != '-')
    return Replies::ERR_UMODEUNKNOWNFLAG(emitter, raw->Argument(1));
  flag = flag.substr(1);
  if (flag.empty())
    return Replies::ERR_UMODEUNKNOWNFLAG(emitter, raw->Argument(1));
  if (flag.size() == 1 || flag.size() == 2) {
    for (std::string::iterator it = flag.begin(); it < flag.end(); it++) {

      switch (*it) {
        case 'i':
          targetChannel->setIsInviteOnly( mode == '+' ? true : false);
          break;
        case 't':
          targetChannel->setIsTopicModifiable( mode == '+' ? false : true);
          break;
        default:
          return Replies::ERR_UMODEUNKNOWNFLAG(emitter, flag);
      }
    }
  } else if (raw->Arguments().size() >= 2) {
    switch (flag.at(0)) {
      case 'k':
        return handleKeyMode(mode);
      case 'o':
        return handleOpMode(mode);
      case 'l':
        return handleLimMode(mode);
      default:
        return Replies::ERR_UMODEUNKNOWNFLAG(emitter, flag);
    }
  }

  return 0;
}
