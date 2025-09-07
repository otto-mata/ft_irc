#include "CommandParser.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <sstream>
#include <unistd.h>

std::vector<std::string>
split(std::string s, const std::string& delimiter)
{
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  tokens.push_back(s);

  return tokens;
}

void
handleInput(User* user)
{
  std::vector<std::string> cmds = split(user->GetIncomingBuffer(), "\r\n");
  for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end();
       ++it) {
    CommandParser::MessageCommand cmd(*it);
    if (cmd.Name() == "CAP")
      user->AppendToOutgoingBuffer("CAP * LS :\r\n");
    else if (cmd.Name() == "NICK") {
      user->SetNickname(cmd.Argument(0));
      if (user->FullyRegistered())
        user->AppendToOutgoingBuffer(":localhost 001 " + user->GetNickname() +
                                     " :Welcome to the IRC Server\r\n");
    } else if (cmd.Name() == "USER") {
      user->SetUsername(cmd.Argument(0));
      user->SetRealName(cmd.Trailing());
      if (user->FullyRegistered())
        user->AppendToOutgoingBuffer(":localhost 001 " + user->GetNickname() +
                                     " :Welcome to the IRC Server\r\n");
    }
  }
}

/**
 * @brief Handle clients I/O
 * @param users Ref to server's UserMap
 * @param buffers Ref to server's BufferMap
 * @param disconnected Ref to the list keeping track of disconnected users
 * @param rfds Read fd set
 * @param wfds Write fd set
 */
static void
manageUserDataReception(UserMap& users,
                        BufferMap& buffers,
                        std::list<User*>& disconnected,
                        fd_set* rfds,
                        fd_set* wfds)
{
  for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
    if (FD_ISSET(it->first, rfds)) {
      char buffer[PKT_SIZE + 1];
      ssize_t rb = recv(it->first, buffer, PKT_SIZE, 0);
      if (rb < 0)
        throw std::runtime_error("Error while receiving data from client.");
      if (rb == 0) {
        disconnected.push_back(it->second);
        buffers[it->first].clear();
        continue;
      }
      buffer[rb] = 0;
      buffers[it->first].append(buffer);
      if (buffers[it->first].find("\r\n") != std::string::npos) {
        it->second->SetIncomingBuffer(buffers[it->first]);
        buffers[it->first].clear();
      }
    }
    std::cout << it->second->GetIncomingBuffer() << std::endl;
    handleInput(it->second);
    if (FD_ISSET(it->first, wfds)) {
      const std::string& userSendBuffer = it->second->GetOutgoingBuffer();
      ssize_t wb =
        send(it->first, userSendBuffer.c_str(), userSendBuffer.size(), 0);
      it->second->ClearOutgoingBuffer();
      if (wb < 0)
        throw std::runtime_error("Error while receiving data from client.");
    }
  }
}

/**
 * @brief Accept a new client if the server is ready
 * @param serverFd Server file descriptor
 * @param users Ref to the server's UserMap
 * @param rfds Read fd set
 */
static void
acceptNewClient(int serverFd, UserMap& users, fd_set* rfds)
{

  SockAddrIn client;
  socklen_t slen = sizeof(client);

  if (FD_ISSET(serverFd, rfds)) {
    int cfd = accept(serverFd, (struct sockaddr*)&client, &slen);
    if (cfd < 0) {
      throw std::runtime_error("Could not accept connection to server");
    }
    users[cfd] = new User(cfd);
  }
}

/**
 * @brief Setup clients' file descriptor to read and/or write when they are
 * ready
 * @param users Ref to server's UserMap
 * @param maxFd Ref to the maximum fd to use in select
 * @param rfds Read fd set
 * @param wfds Write fd set
 */
static void
prepareClientFdsForSelect(UserMap& users,
                          int& maxfd,
                          fd_set* rfds,
                          fd_set* wfds)
{
  for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
    FD_SET(it->first, rfds);
    if (it->second->ReadyToSend())
      FD_SET(it->first, wfds);
    if (it->first > maxfd)
      maxfd = it->first;
  }
}

/**
 * @brief Handle clients' disconnection
 * @param users Ref to server's UserMap
 * @param disconnected Ref to the list keeping track of disconnected users
 */

static void
handleClientDisconnection(UserMap& users, std::list<User*>& disconnected)
{
  for (std::list<User*>::iterator it = disconnected.begin();
       it != disconnected.end();
       it++) {
    users.erase((*it)->Fileno());
    delete *it;
  }
}

void
Server::Start(void)
{
  fd_set rfds;
  fd_set wfds;
  timeval tv;
  int cfd;

  tv.tv_sec = 0;
  tv.tv_usec = (long)(POLL_INTERVAL * 100000);
  int fdmax;
  std::list<User*> disconnected;

  while (!mustStop) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(fd, &rfds);
    fdmax = fd;
    disconnected.clear();

    prepareClientFdsForSelect(users, fdmax, &rfds, &wfds);
    if (select(fdmax + 1, &rfds, &wfds, 0, &tv) < 0)
      throw std::runtime_error("Fatal select() error");
    acceptNewClient(fd, users, &rfds);
    manageUserDataReception(users, buffers, disconnected, &rfds, &wfds);
    handleClientDisconnection(users, disconnected);
  }
}
