
#include "Pass.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Pass::Pass(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Pass::ValidateInput(void)
{
  return 0;
}

int
Commands::Pass::Execute(void)
{
  return 0;
}
