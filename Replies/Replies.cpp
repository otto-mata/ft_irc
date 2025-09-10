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
Replies::SendReply221ToUser(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 221 " + user->GetNickname() +
                                 " +");
  return 221;
}

int
Replies::SendReply501ToUserForFlag(Core::User* user, const std::string& flag)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 501 " + user->GetNickname() +
                                 " " + flag + " :Unknown MODE flag");
  return 501;
}

int
Replies::SendReply442ToUserForChannelName(Core::User* user,
                                          const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 442 " + channel +
                                 ": You are not in this channel");
  return 442;
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

int
Replies::SendReply403ToUserForChannelName(Core::User* user,
                                          const std::string& name)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 403 " + name +
                                 " :No such channel");
  return 403;
}

int
Replies::SendReply482ToUserForChannelName(Core::User* user,
                                          const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 482 " + channel +
                                 " :You are not a channel operator");
  return 482;
}

int
Replies::SendReply331ToUserForChannelName(Core::User* user,
                                          const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 331 " + user->GetNickname() +
                                 " #" + channel + " :No topic set for channel");
  return 0;
}

int
Replies::SendReply332ToUserForChannel(Core::User* user, Core::Channel* channel)
{
  if (user && channel)
    user->AppendToOutgoingBuffer(":localhost 332 " + user->GetNickname() +
                                 " #" + channel->getName() + " :" +
                                 channel->getTopic());
  return 0;
}
