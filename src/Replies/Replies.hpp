#ifndef REPLIES_HPP
#define REPLIES_HPP
#include <Channel.hpp>
#include <User.hpp>
#include <string>

namespace Replies {
int
ERR_NEEDMOREPARAMS(Core::User* user, const std::string& cmdName);
int
ERR_ALREADYREGISTRED(Core::User* user);
int
ERR_UMODEUNKNOWNFLAG(Core::User* user, const std::string& channel);
int
ERR_NOSUCHNICK(Core::User* user, const std::string& nick);
int
ERR_NICKNAMEINUSE(Core::User* user, const std::string& nick);
int
ERR_NOSUCHCHANNEL(Core::User* user, const std::string& name);
int
ERR_NOTONCHANNEL(Core::User* user, const std::string& channel);
int
ERR_CHANOPRIVSNEEDED(Core::User* user, const std::string& channel);
int
ERR_UNKNOWNCOMMAND(Core::User* user, const std::string& cmd);
int
ERR_USERONCHANNEL(Core::User* user,
                  const std::string& nickname,
                  const std::string& channel);
int
ERR_INVITEONLYCHAN(Core::User* user, const std::string& channelName);
int
ERR_BADCHANNELKEY(Core::User* user, const std::string& channelName);
int
ERR_CHANNELISFULL(Core::User* user, const std::string& channelName);
int
ERR_KEYSET(Core::User* user, const std::string& channelName);
int
ERR_PASSWDMISMATCH(Core::User* user);
int
ERR_USERSDONTMATCH(Core::User* user);

int
RPL_NOTOPIC(Core::User* user, const std::string& channel);
int
RPL_TOPIC(Core::User* user, Core::Channel* channel);
int
RPL_UMODEIS(Core::User* user);

} // namespace Replies

#endif
