#include <stdexcept>

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
