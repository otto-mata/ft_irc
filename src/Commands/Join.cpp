#include "Join.hpp"
#include <Channel.hpp>
#include <CustomAlgo.hpp>
#include <ExecutableCommand.hpp>
#include <Replies.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Join::Join(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Join::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
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

  std::vector<std::string> passes;
  std::vector<std::string> channels =
  Algo::String::Split(std::string(raw->Argument(0)), ",");
  if (raw->Arguments().size() == 2)
    passes = Algo::String::Split(std::string(raw->Argument(1)), ",");
  size_t index = 0;
  //std::vector<std::string>::iterator it = channels.begin();
  for (std::vector<std::string>::iterator it = channels.begin();
       it != channels.end();
       it++) {

    if (!SetTargetChannelFromContext(*it)) {
      Core::Channel* tChan = ctx->CreateChannel(*it);
      if (tChan == 0)
        return 1; //! Error during creation
      tChan->SetOwner(emitter);
      if (raw->Arguments().size() == 2)
        tChan->SetPassword(raw->Argument(1));
      SetTargetChannel(tChan);
    }

    if (targetChannel->IsInviteOnly() &&
        !targetChannel->IsUserInWhitelist(emitter)) {
        Replies::ERR_INVITEONLYCHAN(emitter, targetChannel->GetName());
        continue;
    }

    if (targetChannel->IsPasswordProtected()) {
      if (raw->Arguments().size() != 2 || index >= passes.size())
        return Replies::ERR_BADCHANNELKEY(emitter, targetChannel->GetName());
      else if (!targetChannel->TryPassword(passes.at(index))) {
        index++;
		return Replies::ERR_BADCHANNELKEY(emitter, targetChannel->GetName());
      }
    }

    targetChannel->Broadcast(":" + emitter->FullIdentityString() + " JOIN #" +
                             targetChannel->GetName(), emitter);
    // emitter->AppendToOutgoingBuffer(":" + emitter->FullIdentityString() + " JOIN #" + targetChannel->GetName());
    targetChannel->AddUser(emitter);
    std::string welcomeBuffer =
      ":" + ctx->Hostname() + " 332 " + emitter->GetNickname() + " #" +
      targetChannel->GetName() + " :" + targetChannel->GetTopic() + "\r\n";
    welcomeBuffer += ":" + ctx->Hostname() + " 353 " + emitter->GetNickname() +
                     " = #" + targetChannel->GetName() + " :";
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
