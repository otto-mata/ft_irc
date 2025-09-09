
#ifndef CAP_COMMAND_HPP
#define CAP_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Cap : public ExecutableCommand
{
public:
  Cap(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
