
#ifndef MODE_COMMAND_HPP
#define MODE_COMMAND_HPP
#include <ExecutableCommand.hpp>

struct triplet
{
  char Flag;
  std::string Value;
  bool HasValue;
  char Mode;
  triplet(char c, std::string s, bool b, char m)
    : Flag(c)
    , Value(s)
    , HasValue(b)
    , Mode(m)
  {
  }
};

namespace Commands {
class Mode : public ExecutableCommand
{
public:
  Mode(Core::User* Emitter,
       Core::Server* Context,
       CommandParser::MessageCommand* Raw);
  int ValidateInput(void);
  int Execute(void);

private:
  int handleOpMode(triplet& t);
  int handleKeyMode(triplet& t);
  int handleLimMode(triplet& t);
};
}
#endif
