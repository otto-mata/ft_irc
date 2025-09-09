#ifndef REPLIES_HPP
#define REPLIES_HPP
#include "../User.hpp"
#include <string>

namespace Replies {
void SendReply461ToUserForCommand(Core::User* user, const std::string& cmdName);
} // namespace Replies

#endif
