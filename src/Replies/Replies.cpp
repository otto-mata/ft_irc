#include "Replies.hpp"

int
Replies::ERR_NEEDMOREPARAMS(Core::User* user, const std::string& cmdName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 461 " + user->GetNickname() + " " +
                                 cmdName + " :Not enough parameters");
  return 461;
}

int
Replies::ERR_ALREADYREGISTRED(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 462 " + user->GetNickname() +
                                 " :You may not re-register");
  return 462;
}

int
Replies::RPL_UMODEIS(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 221 " + user->GetNickname() +
                                 " +");
  return 221;
}

int
Replies::RPL_WHOISUSER(Core::User* user, Core::User* target)
{
  if (user && target)
    user->AppendToOutgoingBuffer(
      ":localhost 311 " + user->GetNickname() + " " + target->GetNickname() +
      " " + target->GetUsername() + " * * :" + target->GetRealName());
  return 311;
}

int
Replies::RPL_ENDOFWHOIS(Core::User* user, Core::User* target)
{
  if (user && target)
    user->AppendToOutgoingBuffer(":localhost 318 " + user->GetNickname() + " " +
                                 target->GetNickname() + " :End of WHOIS\r\n");
  return 318;
}

int
Replies::RPL_ENDOFWHOWAS(Core::User* user, Core::User* target)
{
  if (user && target)
    user->AppendToOutgoingBuffer(":localhost 369 " + user->GetNickname() + " " +
                                 target->GetNickname() + " :End of WHOWAS\r\n");
  return 369;
}

int
Replies::RPL_WHOWASUSER(Core::User* user, Core::User* target)
{
  if (user && target)
    user->AppendToOutgoingBuffer(
      ":localhost 314 " + user->GetNickname() + " " + target->GetNickname() +
      " " + target->GetUsername() + " * * :" + target->GetRealName());
  return 314;
}

int
Replies::ERR_WASNOSUCHNICK(Core::User* user, const std::string& nick)
{

  if (user)
    user->AppendToOutgoingBuffer(":localhost 406 " + user->GetNickname() + " " +
                                 nick + " :There was no such nickname");
  return 406;
}

int
Replies::ERR_UMODEUNKNOWNFLAG(Core::User* user, const std::string& flag)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 501 " + user->GetNickname() + " " +
                                 flag + " :Unknown MODE flag");
  return 501;
}

int
Replies::ERR_NOTONCHANNEL(Core::User* user, const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 442 " + user->GetNickname() + " " +
                                 channel + ": You are not in this channel");
  return 442;
}

int
Replies::ERR_NOSUCHNICK(Core::User* user, const std::string& nick)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 401 " + user->GetNickname() + " " +
                                 nick + " :No such nick/channel");
  return 401;
}

int
Replies::ERR_NICKNAMEINUSE(Core::User* user, const std::string& nick)
{
  if (user) {
    std::string nickname =
      user->GetNickname().size() > 0 ? user->GetNickname() : "*";
    user->AppendToOutgoingBuffer(":" + user->FullIdentityString() + " 433 " +
                                 nickname + " " + nick +
                                 " :Nickname already in use");
  }
  return 433;
}

int
Replies::ERR_NOSUCHCHANNEL(Core::User* user, const std::string& name)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 403 " + user->GetNickname() + " " +
                                 name + " :No such channel");
  return 403;
}

int
Replies::ERR_CHANOPRIVSNEEDED(Core::User* user, const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 482 " + user->GetNickname() + " " +
                                 channel + " :You are not a channel operator");
  return 482;
}

int
Replies::ERR_UNKNOWNCOMMAND(Core::User* user, const std::string& cmd)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 421 " + user->GetNickname() + " " +
                                 cmd + " :Unknown command");

  return 421;
}

int
Replies::ERR_USERONCHANNEL(Core::User* user,
                           const std::string& nickname,
                           const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 443 " + user->GetNickname() + " " +
                                 nickname + " #" + channel +
                                 " :is already on channel");

  return 443;
}

int
Replies::ERR_INVITEONLYCHAN(Core::User* user, const std::string& channelName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 473 " + user->GetNickname() + " " +
                                 channelName + " :Cannot join channel (+i)");

  return 473;
}

int
Replies::ERR_BADCHANNELKEY(Core::User* user, const std::string& channelName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 475 " + user->GetNickname() + " " +
                                 channelName + " :Cannot join channel (+k)");

  return 475;
}

int
Replies::ERR_CHANNELISFULL(Core::User* user, const std::string& channelName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 471 " + user->GetNickname() + " " +
                                 channelName + " :Cannot join channel (+l)");
  return 471;
}

int
Replies::ERR_KEYSET(Core::User* user, const std::string& channelName)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 467 " + user->GetNickname() + " " +
                                 channelName + " :Channel key already set");
  return 467;
}

int
Replies::ERR_PASSWDMISMATCH(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 475 " + user->GetNickname() +
                                 " :Password incorrect");
  return 475;
}

int
Replies::ERR_USERSDONTMATCH(Core::User* user)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 502 " + user->GetNickname() +
                                 " :Cannot change MODE for another user");
  return 502;
}

int
Replies::RPL_NOTOPIC(Core::User* user, const std::string& channel)
{
  if (user)
    user->AppendToOutgoingBuffer(":localhost 331 " + user->GetNickname() +
                                 " " + channel + " :No topic set for channel");
  return 0;
}

int
Replies::RPL_TOPIC(Core::User* user, Core::Channel* channel)
{
  if (user && channel)
    user->AppendToOutgoingBuffer(":localhost 332 " + user->GetNickname() +
                                 " " + channel->GetName() + " :" +
                                 channel->GetTopic());
  return 0;
}
