#include "Ping.hpp"
#include "../ExecutableCommand.hpp"
#include "../Replies/Replies.hpp"
#include "../Server.hpp"
#include "../User.hpp"
#include "../Replies/Replies.hpp"

Commands::Ping::Ping(Core::User* Emitter,
                     Core::Server* Context,
                     CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Ping::ValidateInput(void)
{
  return 0;
}

int
Commands::Ping::Execute(void)
{
  if (!raw->HasArguments() || raw->Arguments().empty() ||
      (!raw->Arguments().empty() && raw->Argument(0) == ctx->Hostname())) {
    emitter->AppendToOutgoingBuffer(":" + ctx->Hostname() + " PONG " +
                                    emitter->GetNickname());
    return (0);
  }
  if (!SetTargetUserFromContext(raw->Argument(0)))
    return Replies::ERR_NOSUCHNICK(emitter, raw->Argument(0));
  targetUser->AppendToOutgoingBuffer(":" + emitter->GetNickname() + " PING " +
                                     targetUser->GetNickname());
  return 0;
}
