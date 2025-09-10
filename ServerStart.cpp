#include "Channel.hpp"
#include "CommandParser.hpp"
#include "CustomAlgo.hpp"
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <sstream>
#include <unistd.h>

void
handleInput(Core::User* user, Core::Server* ctx)
{
  std::vector<std::string> cmds =
    Algo::String::Split(user->GetIncomingBuffer(), "\r\n");
  for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end();
       ++it) {
    CommandParser::MessageCommand msgCmd(*it);
    ExecutableCommand* cmd = msgCmd.ToExecutable(user, ctx);
    if (!cmd)
      continue;
    if (cmd->ValidateInput() == Commands::VALID_ARGS)
      cmd->Execute();
    delete cmd;
  }
  user->ClearIncomingBuffer();
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
manageUserDataReception(Core::UserMap& users,
                        Core::BufferMap& buffers,
                        std::list<Core::User*>& disconnected,
                        fd_set* rfds,
                        fd_set* wfds,
                        Core::Server* ctx)
{
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); ++it) {
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
    if (!it->second->GetIncomingBuffer().empty() ||
        it->second->GetIncomingBuffer() != "\r\n")
      handleInput(it->second, ctx);
    if (it->second->ReadyToSend())
      FD_SET(it->first, wfds);
    if (FD_ISSET(it->first, wfds)) {
      const std::string& userSendBuffer = it->second->GetOutgoingBuffer();
      ssize_t wb =
        send(it->first, userSendBuffer.c_str(), userSendBuffer.size(), 0);
      it->second->ClearOutgoingBuffer();
      if (wb < 0)
        throw std::runtime_error("Error while receiving data from client.");
    }
    if (it->second->MustBeDeleted()) {
      disconnected.push_back(it->second);
      buffers[it->first].clear();
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
acceptNewClient(int serverFd, Core::UserMap& users, fd_set* rfds)
{

  SockAddrIn client;
  socklen_t slen = sizeof(client);

  if (FD_ISSET(serverFd, rfds)) {
    int cfd = accept(serverFd, (struct sockaddr*)&client, &slen);
    if (cfd < 0) {
      throw std::runtime_error("Could not accept connection to server");
    }
    users[cfd] = new Core::User(cfd);
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
prepareClientFdsForSelect(Core::UserMap& users,
                          int& maxfd,
                          fd_set* rfds,
                          fd_set* wfds)
{
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); ++it) {
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
handleClientDisconnection(Core::UserMap& users,
                          std::list<Core::User*>& disconnected,
                          Core::Server* ctx)
{
  for (std::list<Core::User*>::iterator it = disconnected.begin();
       it != disconnected.end();
       it++) {
    ctx->Broadcast(
      ":" + (*it)->GetNickname() + " QUIT :" + (*it)->GetQuitMessage(), (*it));
    users.erase((*it)->Fileno());
    delete *it;
  }
}

void
Core::Server::Start(void)
{
  fd_set rfds;
  fd_set wfds;
  timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = (long)(POLL_INTERVAL * 100000);
  int fdmax;
  std::list<Core::User*> disconnected;

  while (!mustStop) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(fd, &rfds);
    fdmax = fd;
    disconnected.clear();

    prepareClientFdsForSelect(users, fdmax, &rfds, &wfds);
    if (select(fdmax + 1, &rfds, &wfds, 0, &tv) < 0) {
      if (mustStop)
        break;
      else
        throw std::runtime_error("Fatal select() error");
    }
    acceptNewClient(fd, users, &rfds);
    manageUserDataReception(users, buffers, disconnected, &rfds, &wfds, this);
    handleClientDisconnection(users, disconnected, this);
  }
  std::cout << "Exiting server." << std::endl;
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); it++) {
    delete it->second;
  }
  for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
       it++) {
    delete it->second;
  }
}
