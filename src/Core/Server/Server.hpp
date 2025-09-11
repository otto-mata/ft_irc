#ifndef SERVER_HPP
#define SERVER_HPP

#include <Common.hpp>
#include <Logger.hpp>
#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifndef BACKLOG
#define BACKLOG 5
#endif

#ifndef POLL_INTERVAL
#define POLL_INTERVAL 500
#endif

namespace Core {
class User;
class Channel;
typedef std::map<int, Core::User*> UserMap;
typedef std::map<std::string, Core::Channel*> ChannelMap;
typedef std::map<int, std::string> BufferMap;

class Server
{
private:
  unsigned short port;
  int fd;
  fd_set rfds;
  fd_set wfds;
  int maxfd;
  std::string password;
  SockAddrIn in;
  Core::UserMap users;
  Core::ChannelMap channels;
  Core::BufferMap buffers;
  std::string hostName;
  Logging::Engine log;
  std::vector<Core::User*> disconnected;
  void handleClientDisconnection();
  void prepareClientFdsForSelect();
  void acceptNewClient();
  void manageUserDataReception();
  void handleInput(Core::User* user);

public:
  static bool MustStop;
  Server(unsigned short p = 6667, std::string password = "");
  ~Server(void);

  void Start(void);
  void Broadcast(std::string message, Core::User* except = 0);

  Core::Channel* GetChannel(const std::string& name);

  Core::User* FindUserByNickname(const std::string& Nickname);
  Core::User* FindUserByUsername(const std::string& Username);
  Core::Channel* FindChannelByName(const std::string& Name);

  Core::Channel* CreateChannel(const std::string& Name);
  int RemoveChannel(Core::Channel* Chan);

  bool MatchUserByNickname(const std::string& Nickname);
  bool MatchChannelByName(const std::string& Name);

  bool TryPassword(const std::string& attempt);
  bool IsPasswordProtected(void);
  void RemoveUserFromServer(Core::User* user);

  const std::string& Hostname(void);
  static void StopServer(void);

  std::vector<std::string> GetAllChannelNames(void);
};

}
#endif
