
#ifndef QUIT_COMMAND_HPP
#define QUIT_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Quit : public ExecutableCommand
{
public:
  Quit(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
