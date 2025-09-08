
#ifndef NICK_COMMAND_HPP
#define NICK_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Nick : public ExecutableCommand
{
public:
  Nick(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
