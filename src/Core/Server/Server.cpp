#include "Server.hpp"
#include "User.hpp"
#include <cstdio>
#include <sstream>
#include <list>
#include <unistd.h>
#include <csignal>

bool Core::Server::MustStop = false;

Core::Server::Server(unsigned short p, std::string _password)
    : password(_password)
      , hostName("localhost")
      , log("ft_irc") {
    signal(SIGPIPE, SIG_IGN);
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
    if (bind(fd, (struct sockaddr *) &in, sizeof(in)) < 0) {
        close(fd);
        throw std::runtime_error("Could not bind socket");
    }
    if (listen(fd, BACKLOG) < 0) {
        close(fd);
        throw std::runtime_error("Could not start listenning on socket");
    }
    MustStop = false;
    std::ostringstream startLog;
    startLog << "Server started on port " << p << ".";
    log.info(startLog.str());
}

Core::Server::~Server() {
    log.info("Server exited.");
}
