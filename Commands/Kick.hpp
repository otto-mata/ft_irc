
#ifndef KICK_COMMAND_HPP
#define KICK_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Kick : public ExecutableCommand
{
public:
  Kick(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
