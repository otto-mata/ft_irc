#include "Server.hpp"
#include "User.hpp"

void
Core::Server::Broadcast(std::string message, Core::User* except)
{
  for (UserMap::iterator it = users.begin(); it != users.end(); ++it)
    if (it->second != except)
      it->second->SetOutgoingBuffer(message);
}
