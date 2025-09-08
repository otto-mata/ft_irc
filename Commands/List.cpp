
#include "List.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::List::List(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::List::ValidateInput(void)
{
  return 0;
}

int
Commands::List::Execute(void)
{
  return 0;
}
