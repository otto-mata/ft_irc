#include "Replies.hpp"

void
Replies::SendReply461ToUserForCommand(Core::User* user, const std::string& cmdName)
{
  if (!user)
    return;
  user->AppendToOutgoingBuffer(":localhost 461 " + user->GetNickname() + " " +
                               cmdName + " :Not enough parameters.");
}
