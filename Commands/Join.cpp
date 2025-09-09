
#include "Join.hpp"
#include "../Channel.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"
#include "../Replies/Replies.hpp"

Commands::Join::Join(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Join::ValidateInput(void)
{
  if (raw->Arguments().size() == 0) {
    Replies::SendReply461ToUserForCommand(emitter, raw->Name());
    return 461; //! Invalid arguments count (not enough)
  }
  if (raw->Argument(0).find(',') == std::string::npos) {
    if (!SetTargetChannelFromContext(raw->Argument(0))) {
      SetTargetChannel(0);
    }
  } else {
    return 3; //! multi channel Not implemented yet (#chan0,#chan1,...)
  }
  return 0;
}

int
Commands::Join::Execute(void)
{
  if (targetChannel == 0) {
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
    emitter->AppendToOutgoingBuffer(":localhost 473 " + emitter->GetNickname() +
                                    " #" + targetChannel->getName() +
                                    " :Cannot join invite-only channel");
    return 2; //! User not invited to channel
  }
  if (targetChannel->getIsPasswordProtected()) {
    if (raw->Arguments().size() != 2)
      return 3; //! No password provided for channel
    else if (!targetChannel->tryPassword(raw->Argument(1)))
      return 4; //! Invalid password;
  }
  std::string welcomeBuffer = ":" + emitter->GetNickname() + " JOIN " +
                                  targetChannel->getName() + "\r\n";
  welcomeBuffer += ":" + emitter->GetNickname() + " 332 " + emitter->GetNickname() + " " +
    targetChannel->getName() + " :" + targetChannel->getTopic() + "\r\n";
  welcomeBuffer += ":" + emitter->GetNickname() + " 353 " +
                                  emitter->GetNickname() + " = " +
                                  targetChannel->getName() + " :";
  const Users& tChanUsers = targetChannel->GetUsers();
  for (Users::iterator it = tChanUsers.begin(); it != tChanUsers.end(); it++) {
    welcomeBuffer += (*it)->GetNickname() + " ";
  }
  welcomeBuffer += "\r\n";
  targetChannel->Broadcast(welcomeBuffer);
  return 0;
}
