
#ifndef WHOWAS_COMMAND_HPP
#define WHOWAS_COMMAND_HPP
#include <ExecutableCommand.hpp>

namespace Commands {
class Whowas : public ExecutableCommand
{
public:
  Whowas(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
