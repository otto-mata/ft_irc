
#include "Privmsg.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Privmsg::Privmsg(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Privmsg::ValidateInput(void)
{
  return 0;
}

int
Commands::Privmsg::Execute(void)
{
  return 0;
}
