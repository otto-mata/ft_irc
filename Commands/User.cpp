
#include "User.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::User::User(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::User::ValidateInput(void)
{
  return 0;
}

int
Commands::User::Execute(void)
{
  return 0;
}
