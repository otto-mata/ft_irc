
#ifndef PING_COMMAND_HPP
#define PING_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Ping : public ExecutableCommand
{
public:
  Ping(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
