#ifndef REPLIES_HPP
#define REPLIES_HPP
#include "../Channel.hpp"
#include "../User.hpp"
#include <string>

namespace Replies {
int
SendReply461ToUserForCommand(Core::User* user, const std::string& cmdName);
int
SendReply462ToUser(Core::User* user);
int
SendReply221ToUser(Core::User* user);
int
SendReply501ToUserForFlag(Core::User* user, const std::string& channel);
int
SendReply401ToUserForNickname(Core::User* user, const std::string& nick);
int
SendReply403ToUserForChannelName(Core::User* user, const std::string& name);
int
SendReply442ToUserForChannelName(Core::User* user, const std::string& channel);
int
SendReply482ToUserForChannelName(Core::User* user, const std::string& channel);
int
SendReply331ToUserForChannelName(Core::User* user, const std::string& channel);
int
SendReply332ToUserForChannel(Core::User* user, Core::Channel* channel);

} // namespace Replies

#endif
