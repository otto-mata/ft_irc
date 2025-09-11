#include "Server.hpp"
#include <signal.h>

void
control(__attribute_maybe_unused__ int n)
{
  Core::Server::StopServer();
}

int
main()
{
  Core::Server srv;
  signal(SIGQUIT, SIG_IGN);
  signal(SIGINT, control);
  try {
    srv.Start();
  } catch (const std::runtime_error& e) {
    if (Core::Server::MustStop)
      return (0);
  }
  return (0);
}
