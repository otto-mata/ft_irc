
#include "Quit.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Quit::Quit(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Quit::ValidateInput(void)
{
  return 0;
}

int
Commands::Quit::Execute(void)
{
  return 0;
}
