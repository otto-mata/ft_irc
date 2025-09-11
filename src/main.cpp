#include "Server.hpp"
#include <Logger.hpp>
#include "Args.hpp"
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
main(int argc, char **argv)
{
  Core::Arguments args(argc, argv);
  if (args.parseArgs())
    return (1);
  log.debug("Starting server...");
  Core::Server srv(args.port, args.password);
  log.debug("Setting up signal interceptors...");
  signal(SIGQUIT, SIG_IGN);
  signal(SIGINT, control);
  try {
    srv.Start();
  } catch (const std::runtime_error& e) {
    log.fatal(e.what());
  }
  return (0);
}
