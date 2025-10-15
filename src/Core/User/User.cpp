#include "User.hpp"
#include <CustomAlgo.hpp>
#include <algorithm>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

std::string Core::User::MotdFile = "./.motd";

Core::User::User(int fd)
  : _fd(fd)
  , _quitMessage("Left the chat")
  , _hasNick(false)
  , _hasUser(false)
  , _hasSentPassword(false)
  , _isValidPassword(false)
  , _disconnected(true)
  , _capabilitiesNegotiationFinished(true)
  , _toDelete(false)
{
  ResolveHostname();
  log = Logging::Engine("ft_irc|user|" + RemoteConnectionString());
}

Core::User::~User(void)
{
  close(_fd);
  log.debug("Connection to remote client closed.");
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
Core::User::GetEscapedIncomingBuffer(void)
{
  return Algo::String::EscapeSequence(_incomingBuffer);
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

std::string
Core::User::GetEscapedOutgoingBuffer(void)
{
  return Algo::String::EscapeSequence(_outgoingBuffer);
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

bool
Core::User::CompletedRegistrationRoutine(const std::string& from)
{
  if (!FullyRegistered() || _disconnected == false)
    return false;
  AppendToOutgoingBuffer(":" + from + " 001 " + GetNickname() +
                         " :Hello! Welcome to IRC " + FullIdentityString());
  AppendToOutgoingBuffer(":" + from + " 002 " + GetNickname() +
                         " :Your host is ft_irc, running 0.0.1a");
  AppendToOutgoingBuffer(":" + from + " 003 " + GetNickname() +
                         " :This server was created at some point");
  AppendToOutgoingBuffer(":" + from + " 004 " + GetNickname() + " " + from +
                         " ft_irc 0.0.1a * itklo klo");
  sendMotd();
  _disconnected = false;
  return true;
}

void
Core::User::SetPasswordReceived(bool state)
{
  _hasSentPassword = state;
}

bool
Core::User::SetPasswordValid(bool state)
{
  _isValidPassword = state;
  return state;
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
  // char hostname[1024];
  // char service[32];

  if (getpeername(_fd, (struct sockaddr*)&addr, &len) == -1)
    return;
  _ipAdress = std::string(inet_ntoa(addr.sin_addr));

  // if (getnameinfo((struct sockaddr*)&addr,
  //                 sizeof(addr),
  //                 hostname,
  //                 sizeof(hostname),
  //                 service,
  //                 sizeof(service),
  //                 0)) {
  //   _hostname = std::string(hostname);
  // } else
  _hostname = _ipAdress;
  _remotePort = ntohs(addr.sin_port);
}

const std::string&
Core::User::GetHostname(void)
{
  return _hostname;
}

std::string
Core::User::RemoteConnectionString(void)
{
  std::ostringstream stream;
  stream << _hostname << ":" << _remotePort;
  return stream.str();
}

void
Core::User::SetQuitMessage(const std::string& msg)
{
  _quitMessage = msg;
}

const std::string&
Core::User::GetQuitMessage(void)
{
  return _quitMessage;
}

void
Core::User::MarkForDeletion(void)
{
  _toDelete = true;
}

bool
Core::User::MustBeDeleted(void)
{
  return _toDelete;
}

void
Core::User::sendMotd(void)
{
  std::ifstream f;
  f.open(Core::User::MotdFile.c_str());
  if (f.fail()) {
    AppendToOutgoingBuffer(":localhost 422 " + _nickname +
                           " :MOTD File is missing");
    return;
  }
  AppendToOutgoingBuffer(":localhost 375 " + _nickname +
                         " :=-=-=-=-=Message of the day=-=-=-=-=");
  std::string line;
  while (!f.eof()) {
    getline(f, line);
    AppendToOutgoingBuffer(":localhost 372 " + _nickname + " :" + line);
  }
  AppendToOutgoingBuffer(":localhost 376 " + _nickname +
                         " :=-=-=-=-=Message of the day=-=-=-=-=");
}

const Core::ChannelMap  &Core::User::getJoinedChanels(void) {
  return joinedChanels;
}

void  Core::User::addChannel(Core::Channel *toadd, const std::string &name) {
  joinedChanels[name] = toadd;

}

void  Core::User::removeChannnel(std::string name) {
  joinedChanels.erase(name);
}

void Core::User::Send(const std::string& message)
{
    std::string fullMessage = message + "\r\n";

    ssize_t sent = send(_fd, fullMessage.c_str(), fullMessage.length(), 0);
    if (sent < 0)
    {
        perror("send failed");
    }
}


