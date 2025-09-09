#ifndef REPLIES_HPP
#define REPLIES_HPP
#include "../User.hpp"
#include <string>

namespace Replies {
int SendReply461ToUserForCommand(Core::User* user, const std::string& cmdName);
int SendReply462ToUser(Core::User* user);
int SendReply401ToUserForNickname(Core::User* user, const std::string& nick);
} // namespace Replies

#endif
