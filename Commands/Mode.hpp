
#ifndef MODE_COMMAND_HPP
#define MODE_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Mode : public ExecutableCommand
{
public:
  Mode(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
