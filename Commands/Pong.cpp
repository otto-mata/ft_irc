
#include "Pong.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"

Commands::Pong::Pong(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw)
  : ExecutableCommand(Emitter, Context, Raw)
{
}

int
Commands::Pong::ValidateInput(void)
{
  return 0;
}

int
Commands::Pong::Execute(void)
{
  return 0;
}
