
#include "Privmsg.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Privmsg::Privmsg(Core::User* Emitter,
                           Core::Server* Context,
                           CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Privmsg::ValidateInput(void)
{
  if (raw->Arguments().size() < 1 || raw->Trailing().empty())
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  return 0;
}

int
Commands::Privmsg::Execute(void)
{
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::SendReply401ToUserForNickname(emitter, raw->Argument(0));
  targetUser->AppendToOutgoingBuffer(":" + emitter->FullIdentityString() +
                                     " PRIVMSG " + targetUser->GetNickname() +
                                     " :" + raw->Trailing());
  return 0;
}
