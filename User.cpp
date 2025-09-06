#include "User.hpp"
#include <unistd.h>

User::User(int fd)
  : _fd(fd)
  , _isRegistered(false)
  , _hasNick(false)
  , _hasUser(false)
  , _hasPass(false)
  , _hasSentPassword(false)
  , _isValidPassword(false)
  , _disconnected(true)
{
}

User::~User(void)
{
  close(_fd);
}

bool
User::sendReady(void)
{
  return _outgoingBuffer.size() > 0;
}

void
User::setIncomingBuffer(const std::string& from)
{
  _incommingBuffer = from;
}

int
User::fileno(void)
{
  return _fd;
}
