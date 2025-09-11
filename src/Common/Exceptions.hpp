#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

#define CompileWarning(msg) __attribute__((unavailable(msg)))

namespace Exceptions {

class NotImplementedException : public std::logic_error
{
public:
  NotImplementedException()
    : std::logic_error("Function not yet implemented.")
  {
  }
};

} // namespace Exceptions
#endif
