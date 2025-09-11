#include "Server.hpp"
#include "Logging/Logger.hpp"
#include <signal.h>
#include <iostream>

Logging::Engine log("Main");

void
control(__attribute_maybe_unused__ int n)
{
  log.info("CTRL-C received");
  Core::Server::StopServer();
}

int
main()
{
  Core::Server srv;
  log.debug("Setting up signal interceptors...");
  signal(SIGQUIT, SIG_IGN);
  signal(SIGINT, control);
  try {
    log.debug("Starting server...");
    srv.Start();
  } catch (const std::runtime_error& e) {
    log.fatal(e.what());
  }
  return (0);
}
