
#include "Whois.hpp"
#include "../ExecutableCommand.hpp"
#include "../Server.hpp"
#include "../User.hpp"

Commands::Whois::Whois(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Whois::ValidateInput(void)
{
  return 0;
}

int
Commands::Whois::Execute(void)
{
  return 0;
}
