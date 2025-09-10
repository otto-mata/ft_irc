
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
    return Replies::ERR_NEEDMOREPARAMS(emitter, raw->Name());
  return 0;
}

int
Commands::Pong::Execute(void)
{
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  targetUser->AppendToOutgoingBuffer(":" + emitter->GetNickname() + " PONG " +
                                     targetUser->GetNickname());
  return (0);
}
