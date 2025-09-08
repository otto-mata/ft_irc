
#ifndef PART_COMMAND_HPP
#define PART_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Part : public ExecutableCommand
{
public:
  Part(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
