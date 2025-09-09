
#include "Pong.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"
#include "../Replies/Replies.hpp"

Commands::Pong::Pong(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Pong::ValidateInput(void)
{
  if (raw->Arguments().empty())
    return Replies::SendReply461ToUserForCommand(emitter, raw->Name());
  return 0;
}

int
Commands::Pong::Execute(void)
{
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return 1; //! No user
  targetUser->AppendToOutgoingBuffer(":" + emitter->GetNickname() + " PONG " +
                                     targetUser->GetNickname());
  return (0);
}
