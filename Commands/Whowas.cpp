
#include "Whowas.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Whowas::Whowas(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Whowas::ValidateInput(void)
{
  return 0;
}

int
Commands::Whowas::Execute(void)
{
  return 0;
}
