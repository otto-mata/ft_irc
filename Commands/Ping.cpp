
#include "Ping.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Ping::Ping(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
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
  return 0;
}
