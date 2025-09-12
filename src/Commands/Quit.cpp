
#include "Quit.hpp"
#include <ExecutableCommand.hpp>
#include <Server.hpp>
#include <User.hpp>

Commands::Quit::Quit(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Quit::ValidateInput(void)
{
  if (!emitter->FullyRegistered() ||(ctx->IsPasswordProtected() && !emitter->HasSentValidPassword()))
    return 1;
  return 0;
}

int
Commands::Quit::Execute(void)
{
  std::string trail = "Left the chat";
  if (raw->HasTrailing())
    trail = raw->Trailing();
  ctx->Broadcast(":"+ emitter->FullIdentityString() + " QUIT :" + trail);
  ctx->RemoveUserFromServer(emitter);
  return 0;
}
