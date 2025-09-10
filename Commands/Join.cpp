#include "Join.hpp"
#include "../Channel.hpp"
#include "../CustomAlgo.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Join::Join(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Join::ValidateInput(void)
{
  if (raw->HasArguments()) {
    std::string argCpy(raw->Argument(0));
    std::vector<std::string> v = Algo::String::Split(argCpy, ",");
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end();
         it++) {
      if ((*it).at(0) != '#')
        return Replies::ERR_NOSUCHNICK(emitter, *it);
    }
  } else
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  return 0;
}

int
Commands::Join::Execute(void)
{

  std::string argCpy(raw->Argument(0));
  std::vector<std::string> v = Algo::String::Split(argCpy, ",");
  for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++) {
    if (!SetTargetChannelFromContext(*it)) {
      Core::Channel* tChan = ctx->CreateChannel(raw->Argument(0));
      if (tChan == 0)
        return 1; //! Error during creation
      tChan->SetOwner(emitter);
      if (raw->Arguments().size() == 2)
        tChan->setPassword(raw->Argument(1));
      SetTargetChannel(tChan);
    }
    if (targetChannel->getIsInviteOnly() &&
        !targetChannel->isUserWhitelist(emitter)) {
      return Replies::ERR_INVITEONLYCHAN(emitter, targetChannel->getName());
    }
    if (targetChannel->getIsPasswordProtected()) {
      if (raw->Arguments().size() != 2)
        return Replies::ERR_PASSWDMISMATCH(emitter);
      else if (!targetChannel->tryPassword(raw->Argument(1)))
        return Replies::ERR_PASSWDMISMATCH(emitter);
    }
    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " JOIN " +
                             targetChannel->getName());
    std::string welcomeBuffer =
      ":" + ctx->Hostname() + " 332 " + emitter->GetNickname() + " " +
      targetChannel->getName() + " :" + targetChannel->getTopic() + "\r\n";
    welcomeBuffer += ":" + ctx->Hostname() + " 353 " + emitter->GetNickname() +
                     " = " + targetChannel->getName() + " :";
    const Users& tChanUsers = targetChannel->GetUsers();
    for (Users::iterator it = tChanUsers.begin(); it != tChanUsers.end();
         it++) {
      welcomeBuffer += (*it)->GetNickname() + " ";
    }
    welcomeBuffer += "\r\n";
    emitter->AppendToOutgoingBuffer(welcomeBuffer);
  }
  return 0;
}
