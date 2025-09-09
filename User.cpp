#include "User.hpp"
#include <unistd.h>

Core::User::User(int fd)
  : _fd(fd)
  , _isRegistered(false)
  , _capabilitiesNegotiationFinished(true)
  , _hasNick(false)
  , _hasUser(false)
  , _hasPass(false)
  , _hasSentPassword(false)
  , _isValidPassword(false)
  , _disconnected(true)
{
}

Core::User::~User(void)
{
  close(_fd);
}

bool
Core::User::ReadyToSend(void)
{
  return _outgoingBuffer.size() > 0;
}

void
Core::User::SetIncomingBuffer(const std::string& from)
{
  _incomingBuffer = from;
}

const std::string&
Core::User::GetIncomingBuffer(void)
{
  return _incomingBuffer;
}

void
Core::User::AppendToOutgoingBuffer(const std::string& from)
{
  std::string buf(from);
  if (from.find("\r\n") == from.npos)
    buf += "\r\n";
  _outgoingBuffer += buf;
}

void
Core::User::AppendToOutgoingBuffer(const char* from)
{
  std::string buf(from);
  if (buf.find("\r\n") == buf.npos)
    buf += "\r\n";
  _outgoingBuffer += buf;
}

int
Core::User::Fileno(void)
{
  return _fd;
}

bool
Core::User::FullyRegistered(void)
{
  return _hasNick && _hasUser;
}

void
Core::User::SetOutgoingBuffer(const char* from)
{
  _outgoingBuffer = from;
}
void
Core::User::SetOutgoingBuffer(const std::string& from)
{
  _outgoingBuffer = from;
}

const std::string&
Core::User::GetOutgoingBuffer(void)
{
  return _outgoingBuffer;
}

void
Core::User::ClearOutgoingBuffer(void)
{
  _outgoingBuffer.clear();
}

void
Core::User::ClearIncomingBuffer(void)
{
  _incomingBuffer.clear();
}

void
Core::User::SetNickname(const std::string& from)
{
  _nickname = from;
  _hasNick = true;
}

const std::string&
Core::User::GetNickname(void)
{
  return _nickname;
}

void
Core::User::SetUsername(const std::string& from)
{
  _username = from;
  _hasUser = true;
}

const std::string&
Core::User::GetUsername(void)
{
  return _username;
}

void
Core::User::SetRealName(const std::string& from)
{
  _realname = from;
}

const std::string&
Core::User::GetRealName(void)
{
  return _realname;
}

void
Core::User::FinishCapabilitiesNegotiation(void)
{
  _capabilitiesNegotiationFinished = true;
}

void
Core::User::PendingCapabilitiesNegotiation(void)
{
  _capabilitiesNegotiationFinished = false;
}

bool
Core::User::HasFinishedCapNeg(void)
{
  return _capabilitiesNegotiationFinished == true;
}
