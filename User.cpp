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

void
User::AppendToOutgoingBuffer(const std::string& from)
{
  _outgoingBuffer += from;
}

void
User::AppendToOutgoingBuffer(const char* from)
{
  _outgoingBuffer += from;
}

int
User::Fileno(void)
{
  return _fd;
}

bool
User::FullyRegistered(void)
{
  return _hasNick && _hasUser;
}

void
User::SetOutgoingBuffer(const char* from)
{
  _outgoingBuffer = from;
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

void
User::ClearOutgoingBuffer(void)
{
  _outgoingBuffer.clear();
}

void
User::ClearIncomingBuffer(void)
{
  _incomingBuffer.clear();
}

void
User::SetNickname(const std::string& from)
{
  _nickname = from;
  _hasNick = true;
}

const std::string&
User::GetNickname(void)
{
  return _nickname;
}

void
User::SetUsername(const std::string& from)
{
  _username = from;
  _hasUser = true;
}

const std::string&
User::GetUsername(void)
{
  return _username;
}

void
User::SetRealName(const std::string& from)
{
  _realname = from;
}

const std::string&
User::GetRealName(void)
{
  return _realname;
}
