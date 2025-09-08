
#ifndef LIST_COMMAND_HPP
#define LIST_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class List : public ExecutableCommand
{
public:
  List(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
