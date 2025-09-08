
#include "Mode.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Mode::Mode(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Mode::ValidateInput(void)
{
  return 0;
}

int
Commands::Mode::Execute(void)
{
  return 0;
}
