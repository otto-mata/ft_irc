
#include "Topic.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Topic::Topic(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Topic::ValidateInput(void)
{
  return 0;
}

int
Commands::Topic::Execute(void)
{
  return 0;
}
