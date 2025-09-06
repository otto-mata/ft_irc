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
User::ReadyToSend(void)
{
  return _outgoingBuffer.size() > 0;
}

void
User::SetIncomingBuffer(const std::string& from)
{
  _incomingBuffer = from;
}

const std::string&
User::GetIncomingBuffer(void)
{
  return _incomingBuffer;
}

int
User::Fileno(void)
{
  return _fd;
}

void
User::SetOutgoingBuffer(const std::string& from)
{
  _outgoingBuffer = from;
}

const std::string&
User::GetOutgoingBuffer(void)
{
  return _outgoingBuffer;
}
