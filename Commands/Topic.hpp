
#ifndef TOPIC_COMMAND_HPP
#define TOPIC_COMMAND_HPP
#include "../ExecutableCommand.hpp"

namespace Commands {
class Topic : public ExecutableCommand
{
public:
  Topic(User* Emitter, Server* Context, CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);
};
}
#endif
