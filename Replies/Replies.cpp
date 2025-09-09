#include "Replies.hpp"

int
Replies::SendReply461ToUserForCommand(Core::User* user,
                                      const std::string& cmdName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 461 " + user->GetNickname() + " " +
                                 cmdName + " :Not enough parameters");
  return 461;
}

int
Replies::SendReply462ToUser(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 462 " + user->GetNickname() +
                                 " :You may not register");
  return 462;
}

int
Replies::SendReply401ToUserForNickname(Core::User* user,
                                       const std::string& nick)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 401 " + nick +
                                 " :No such nick/channel");
  return 401;
}
