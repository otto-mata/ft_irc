
#include "Mode.hpp"
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Mode::Mode(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Mode::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  if (!raw->HasArguments()) {
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  }
  return 0;
}

int
Commands::Mode::handleOpMode(triplet& t)
{
  if (!t.HasValue)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (!SetTargetUserFromContext(t.Value))
    return Replies::ERR_NOSUCHNICK(emitter, t.Value);
  if (!targetChannel->IsUser(targetUser))
    return Replies::ERR_NOSUCHNICK(emitter, targetUser->GetNickname());
  if (t.Mode == '-') {
    if (!targetChannel->IsAdmin(targetUser))
      return 1;
    targetChannel->RemoveAdmin(targetUser);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " -o " +
                             targetUser->GetNickname());
  }
  if (t.Mode == '+') {
    if (targetChannel->IsAdmin(targetUser))
      return 0;
    targetChannel->AddAdmin(targetUser);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " +o " +
                             targetUser->GetNickname());
  }
  return (0);
}

int
Commands::Mode::handleKeyMode(triplet& t)
{
  if (t.Mode == '+' && !t.HasValue)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (t.Mode == '+') {
    targetChannel->SetPassword(t.Value);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " +k " + t.Value);
  } else if (t.Mode == '-') {
    targetChannel->SetPassword("");
    targetChannel->SetPasswordProtected(false);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " -k ");
  }
  return (0);
}

int
Commands::Mode::handleLimMode(triplet& t)
{
  if (t.Mode == '+' && !t.HasValue)
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  if (t.Mode == '+') {
    size_t n = 0;
    if (!Algo::String::SaferStoul(t.Value, &n))
      return 1;
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " +l " + t.Value);
    targetChannel->SetUserLimit(n);
  } else if (t.Mode == '-') {
    targetChannel->SetUserLimited(false);
    targetChannel->SetUserLimit(~0);
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                             targetChannel->GetName() + " -l");
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
  if (!targetChannel->IsUser(emitter))
    return Replies::ERR_NOTONCHANNEL(emitter, raw->Argument(0));
  if (!targetChannel->IsAdmin(emitter))
    return Replies::ERR_CHANOPRIVSNEEDED(emitter, raw->Argument(0));

  std::string flags(raw->Argument(1));
  std::vector<std::string> args;
  std::vector<triplet> modeUpdates;

  for (size_t i = 2; i < raw->Arguments().size(); i++) {
    args.push_back(raw->Argument(i));
  }
  size_t index = 0;
  char mode = '\0';
  for (std::string::iterator it = flags.begin(); it < flags.end(); it++) {
    if (*it == '-' || *it == '+') {
      mode = *it;
      continue;
    }
    std::string v;
    bool hasV = index < args.size() && *it != 'i' && *it != 't';
    if (hasV) {
      v = args.at(index);
      index++;
    }
    triplet t((*it), v, hasV, mode);
    modeUpdates.push_back(t);
  }
  for (std::vector<triplet>::iterator it = modeUpdates.begin();
       it < modeUpdates.end();
       it++) {
    int execRet = 0;
    if ((*it).Flag == 'i') {
      targetChannel->SetInviteOnly((*it).Mode == '+' ? true : false);
      targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                               targetChannel->GetName() + " " + (*it).Mode +
                               "i");
    } else if ((*it).Flag == 't') {
      targetChannel->SetTopicModifiable((*it).Mode == '+' ? false : true);
      targetChannel->Broadcast(":" + emitter->FullIdentityString() + " MODE #" +
                               targetChannel->GetName() + " " + (*it).Mode +
                               "t");
    } else if ((*it).Flag == 'k')
      execRet = handleKeyMode(*it);
    else if ((*it).Flag == 'o')
      execRet = handleOpMode(*it);
    else if ((*it).Flag == 'l')
      execRet = handleLimMode(*it);
    else
      execRet =
        Replies::ERR_UMODEUNKNOWNFLAG(emitter, std::string() + (*it).Flag);

    if (execRet) {
      return execRet;
    }
  }

  return 0;
}
