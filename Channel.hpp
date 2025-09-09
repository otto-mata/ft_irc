#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>

#define DEFAULT_NAME "unamed_channel"
#define DEFAULT_TOPIC "undefined_topic"
#define DEFAULT_PASSWORD "unsafe_password"
#define DEFAULT_USER_LIMIT_COUNT 42
#define DEFAULT_IS_INVITE_ONLY 0
#define DEFAULT_IS_TOPIC_MODIFIABLE 1
#define DEFAULT_IS_USER_LIMITED 0
#define DEFAULT_IS_PASSWORD_PROTECTED 0

typedef std::set<Core::User*> Users;

namespace Core {

class Channel
{
private:
  std::string _name;
  Users _users;
  Users _whitelist;
  Users _admins;
  Core::User* _owner;
  std::string _topic;
  std::string _password;
  size_t _userLimit;
  bool _isInviteOnly;
  bool _isTopicModifiable;
  bool _isUserLimited;
  bool _isPasswordProtected;

  uint8_t userMatch(const std::string& name);

public:
  Channel();
  Channel(const std::string& channel_name);
  ~Channel();

  /* Basic class settings */

  const std::string& getName();

  int SetOwner(Core::User* Owner);
  Core::User* GetOwner(void);

  bool isUser(const std::string& user_tofind);
  bool isUser(Core::User* user_tofind);
  void addUser(Core::User* user_toadd);
  void removeUser(Core::User* user_toremove);

  bool isUserWhitelist(const std::string& user_tofind);
  bool isUserWhitelist(Core::User* user_tofind);
  void addUserWhitelist(Core::User* user_toadd);
  void removeUserWhitelist(Core::User* user_toremove);

  bool isAdmin(const std::string& admin_tofind);
  bool isAdmin(Core::User* admin_tofind);
  void addAdmin(Core::User* admin_toadd);
  void removeAdmin(Core::User* admin_toremove);

  const std::string& getTopic();
  void setTopic(const std::string& NewTopic);

  bool tryPassword(const std::string& PasswordToTest);
  void setPassword(const std::string& NewPassword);

  size_t getUserLimit();
  void setUserLimit(size_t NewUserLimit);
  size_t getUserCount();

  bool getIsInviteOnly();
  void setIsInviteOnly(bool state);

  bool getIsTopicModifiable();
  void setIsTopicModifiable(bool state);

  bool getIsUserLimited();
  void setIsUserLimited(bool state);

  bool getIsPasswordProtected();
  void setIsPasswordProtected(bool state);

  /* Command calls */

  void joinUser(Core::User* joining_user);
  void joinUser(Core::User* joining_user, const std::string& entered_password);
  void partUser(Core::User* parting_user);
  void inviteUser(Core::User* command_user, Core::User* invited_user);
  void kickUser(Core::User* command_user, Core::User* kicked_user);
  void kickUser(Core::User* command_user,
                Core::User* kicked_user,
                const std::string& kick_reason);
  void activatePassword(const std::string& password_given);
  void modifyTopic(Core::User* command_user, const std::string& new_topic);
  void Broadcast(const std::string& m, Core::User* except = 0);

  const Users& GetUsers(void);
};

} // namespace Core
#endif
