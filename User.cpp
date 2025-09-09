#include "User.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
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

std::string
Core::User::FullIdentityString(void)
{
  return std::string(GetNickname() + "!" + GetUsername() + "@" + GetHostname());
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
  return _hasNick && _hasUser && HasFinishedCapNeg() && HasSentValidPassword();
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

void
Core::User::CompletedRegistrationRoutine(const std::string& from)
{
  if (!FullyRegistered())
    return;
  std::cout << "OK REG" << std::endl;
  AppendToOutgoingBuffer(":" + from + " 001 " + GetNickname() +
                         " :Hello! Welcome to IRC");
  AppendToOutgoingBuffer(":" + from + " 002 " + GetNickname() +
                         " :The current server is " + from);
  AppendToOutgoingBuffer(":" + from + " 003 " + GetNickname() +
                         " :This server was created at some point");
  AppendToOutgoingBuffer(":" + from + " 004 " + GetNickname() + " " + from +
                         " ft_irc v0.0.1a");
}

void
Core::User::SetPasswordReceived(bool state)
{
  _hasSentPassword = state;
}

void
Core::User::SetPasswordValid(bool state)
{
  _isValidPassword = state;
}

bool
Core::User::HasSentValidPassword(void)
{
  return _hasSentPassword && _isValidPassword;
}

void
Core::User::ResolveHostname(void)
{
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);

  if (getpeername(_fd, (struct sockaddr*)&addr, &len) == -1)
    return;
  _ipAdress = std::string(inet_ntoa(addr.sin_addr));
  struct hostent* host =
    gethostbyaddr((const void*)&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
  if (host)
    _hostname = std::string(host->h_name);
  else
    _hostname = _ipAdress;
}

const std::string&
Core::User::GetHostname(void)
{
  return _hostname;
}
