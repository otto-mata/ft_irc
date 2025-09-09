
#ifndef PASS_COMMAND_HPP
#define PASS_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Pass : public ExecutableCommand
{
public:
  Pass(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
