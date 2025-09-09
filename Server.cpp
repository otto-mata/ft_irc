#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <unistd.h>

bool Core::Server::mustStop = false;

Core::Server::Server(unsigned short p)
  : password("password")
  , hostName("localhost")
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

Core::Server::~Server() {}
