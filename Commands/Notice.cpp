
#include "Notice.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Notice::Notice(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Notice::ValidateInput(void)
{
  return 0;
}

int
Commands::Notice::Execute(void)
{
  return 0;
}
