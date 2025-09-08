
#include "Part.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Part::Part(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Part::ValidateInput(void)
{
  return 0;
}

int
Commands::Part::Execute(void)
{
  return 0;
}
