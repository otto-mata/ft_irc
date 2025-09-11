#include <Channel.hpp>
#include "CommandParser.hpp"
#include <CustomAlgo.hpp>
#include "ExecutableCommand.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>

/**
 * @brief Proper input handling function, executing commands as they are
 * received.
 * @param user Command emitter
 * @param ctx Server Context
 */
void
Core::Server::handleInput(Core::User* user)
{
  if (user->GetIncomingBuffer().size() == 0)
    return;
  std::vector<std::string> cmds =
    Algo::String::Split(user->GetIncomingBuffer(), "\r\n");
  for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end();
       ++it) {
    CommandParser::MessageCommand msgCmd(*it);
    if (msgCmd.Name().size() == 0)
      continue;
    ExecutableCommand* cmd = msgCmd.ToExecutable(user, this);
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
 * @param ctx Server Context
 */
void
Core::Server::manageUserDataReception()
{
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); ++it) {
    if (FD_ISSET(it->first, &rfds)) {
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
        log.debug("[" + it->second->RemoteConnectionString() + "] => [" +
                  Hostname() + "] " + it->second->GetEscapedIncomingBuffer());
        buffers[it->first].clear();
      }
    }
    handleInput(it->second);
    if (FD_ISSET(it->first, &wfds)) {
      std::vector<std::string> cmds =
        Algo::String::Split(it->second->GetOutgoingBuffer(), "\r\n");
      for (std::vector<std::string>::iterator part = cmds.begin();
           part != cmds.end();
           ++part) {
        (*part).append("\r\n");
        log.debug("[" + Hostname() + "] => [" +
                  it->second->RemoteConnectionString() + "] " +
                  Algo::String::EscapeSequence(*part));
        ssize_t wb = send(it->first, (*part).c_str(), (*part).size(), 0);
        if (wb < 0)
          throw std::runtime_error("Error while receiving data from client.");
      }
      it->second->ClearOutgoingBuffer();
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
void
Core::Server::acceptNewClient()
{

  SockAddrIn client;
  socklen_t slen = sizeof(client);

  if (FD_ISSET(fd, &rfds)) {
    int cfd = accept(fd, (struct sockaddr*)&client, &slen);
    if (cfd < 0) {
      throw std::runtime_error("Could not accept connection to server");
    }
    users[cfd] = new Core::User(cfd);
    log.debug("New connection from " + users[cfd]->RemoteConnectionString());
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
void
Core::Server::prepareClientFdsForSelect()
{
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); ++it) {
    FD_SET(it->first, &rfds);
    if (it->second->ReadyToSend())
      FD_SET(it->first, &wfds);
    if (it->first > maxfd)
      maxfd = it->first;
  }
}

/**
 * @brief Handle clients' disconnection
 * @param users Ref to server's UserMap
 * @param disconnected Ref to the list keeping track of disconnected users
 * @param ctx Server Context
 */

void
Core::Server::handleClientDisconnection()
{
  for (std::vector<Core::User*>::iterator it = disconnected.begin();
       it != disconnected.end();
       it++) {
    Broadcast(":" + (*it)->GetNickname() + " QUIT :" + (*it)->GetQuitMessage(),
              (*it));
    users.erase((*it)->Fileno());
    delete *it;
  }
}

void
Core::Server::Start(void)
{
  timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = (long)(POLL_INTERVAL * 100000);

  while (!MustStop) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(fd, &rfds);
    maxfd = fd;
    disconnected.clear();

    prepareClientFdsForSelect();
    if (select(maxfd + 1, &rfds, &wfds, 0, &tv) < 0) {
      // Here, we asssume MustStop is a marker for CTRL-C.
      // Because we can't use <errno.h>, we cannot be sure that
      // select() returned -1 because of a caught signal (which would set errno to EINTR).
      // This is a bit hacky, but hey...
      if (MustStop) 
        break;
      else
        throw std::runtime_error("Fatal select() error");
    }
    try {
    acceptNewClient();
    manageUserDataReception();
    handleClientDisconnection();
    } catch (const std::runtime_error& e)
    {
      log.fatal(e.what());
      Core::Server::StopServer();
    }
  }
  log.info("Stopping...");
  close(fd);
  log.info("Deleting users...");
  for (Core::UserMap::iterator it = users.begin(); it != users.end(); it++) {
    delete it->second;
  }
  log.info("Deleting channels...");
  for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
       it++) {
    delete it->second;
  }
  log.info("Server stopped.");
}
