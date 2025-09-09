
#ifndef NOTICE_COMMAND_HPP
#define NOTICE_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Notice : public ExecutableCommand
{
public:
  Notice(Core::User* Emitter, Core::Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
