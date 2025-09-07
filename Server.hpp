#ifndef SERVER_HPP
#define SERVER_HPP

#include "Common.hpp"
#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>

#ifndef BACKLOG
#define BACKLOG 5
#endif

#ifndef POLL_INTERVAL
#define POLL_INTERVAL 500
#endif

class User;
class Channel;

typedef std::map<int, User*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;
typedef std::map<int, std::string> BufferMap;

class Server
{
private:
  unsigned short port;
  int fd;
  bool mustStop;
  SockAddrIn in;
  UserMap users;
  ChannelMap channels;
  BufferMap buffers;

public:
  Server(unsigned short p = 6667);
  ~Server(void);

  void Start(void);
  void Broadcast(std::string message, User* except = 0);
  User* FindUserByNickname(const std::string& Nickname);
  Channel* FindChannelByName(const std::string& Name);
};

#endif
