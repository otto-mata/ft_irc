#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <list>

Server::Server(unsigned short p)
{
  port = htons(p);
  fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (fd < 0)
    throw std::runtime_error("Could not create server socket");
  int optValue = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) <
      0) {
    close(fd);
    throw std::runtime_error("Could not set socket option SO_REUSEADDR");
  }
  in.sin_addr.s_addr = INADDR_ANY;
  in.sin_port = port;
  in.sin_family = AF_INET;
  for (int i = 0; i < 8; i++)
    in.sin_zero[i] = 0;
  if (bind(fd, (struct sockaddr*)&in, sizeof(in)) < 0) {
    close(fd);
    throw std::runtime_error("Could not bind socket");
  }
  if (listen(fd, BACKLOG) < 0) {
    close(fd);
    throw std::runtime_error("Could not start listenning on socket");
  }
  mustStop = false;
  std::cout << "Server started on port " << p << "." << std::endl;
}

Server::~Server() {}

void
Server::serve(void)
{
  SockAddrIn client;
  fd_set rfds;
  fd_set wfds;
  timeval tv;
  int cfd;

  tv.tv_sec = 0;
  tv.tv_usec = (long)(POLL_INTERVAL * 100000);
  int fdmax;
  socklen_t slen = sizeof(client);
  std::list<User*> disconnected;

  while (!mustStop) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(fd, &rfds);
    fdmax = fd;
    disconnected.clear();

    for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
      FD_SET(it->first, &rfds);
      if (it->second->sendReady())
        FD_SET(it->first, &wfds);
      if (it->first > fdmax)
        fdmax = it->first;
    }
    int sel = select(fdmax + 1, &rfds, &wfds, 0, &tv);
    if (sel < 0) {
      perror("SocketServer::serve - select");
      return;
    }

    if (FD_ISSET(fd, &rfds)) {
      cfd = accept(fd, (struct sockaddr*)&client, &slen);
      if (cfd < 0) {
        perror("SocketServer::serve - accept");
        return;
      }
      users[cfd] = new User(cfd);
    }
    for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
      if (FD_ISSET(it->first, &rfds)) {
        char buffer[PKT_SIZE + 1];
        ssize_t rb = recv(it->first, buffer, PKT_SIZE, 0);
        if (rb < 0)
          throw std::runtime_error("Error while receiving data from client.");
        if (rb == 0) {
          disconnected.push_back(it->second);
          continue;
        }
        buffer[rb] = 0;
        buffers[it->first].append(buffer);
        if (buffers[it->first].find("\r\n") != std::string::npos) {
          it->second->setIncomingBuffer(buffers[it->first]);
          std::cout << "Full data received: " << buffers[it->first] << std::endl;
          buffers[it->first].clear();
        }
      }
      if (FD_ISSET(it->first, &wfds)) {
        // Ready to send
      }
    }
    for (std::list<User*>::iterator it = disconnected.begin(); it != disconnected.end(); it++)
    {
      users.erase((*it)->fileno());
      delete *it;
    }
    
  }
}
