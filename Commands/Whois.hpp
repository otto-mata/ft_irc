
#ifndef WHOIS_COMMAND_HPP
#define WHOIS_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Whois : public ExecutableCommand
{
public:
  Whois(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
