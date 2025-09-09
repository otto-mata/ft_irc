#include "Channel.hpp"
#include "CustomAlgo.hpp"
#include "UserPredicates.hpp"

Core::Channel::Channel()
  : _name(DEFAULT_NAME)
  , _topic(DEFAULT_TOPIC)
  , _password(DEFAULT_PASSWORD)
  , _userLimit(DEFAULT_USER_LIMIT_COUNT)
  , _isInviteOnly(DEFAULT_IS_INVITE_ONLY)
  , _isTopicModifiable(DEFAULT_IS_TOPIC_MODIFIABLE)
  , _isUserLimited(DEFAULT_IS_USER_LIMITED)
  , _isPasswordProtected(DEFAULT_IS_PASSWORD_PROTECTED)
{
}

Core::Channel::Channel(const std::string& channel_name)
  : _name(channel_name)
  , _topic(DEFAULT_TOPIC)
  , _password(DEFAULT_PASSWORD)
  , _userLimit(DEFAULT_USER_LIMIT_COUNT)
  , _isInviteOnly(DEFAULT_IS_INVITE_ONLY)
  , _isTopicModifiable(DEFAULT_IS_TOPIC_MODIFIABLE)
  , _isUserLimited(DEFAULT_IS_USER_LIMITED)
  , _isPasswordProtected(DEFAULT_IS_PASSWORD_PROTECTED)
{
}

Core::Channel::~Channel() {}

const std::string&
Core::Channel::getName()
{
  return _name;
}

int
Core::Channel::SetOwner(Core::User* Owner)
{
  if (_owner != 0)
    return 0;
  _owner = Owner;
  addAdmin(Owner);
  return 1;
}

Core::User*
Core::Channel::GetOwner(void)
{
  return _owner;
}

uint8_t
Core::Channel::userMatch(const std::string& name)
{
  bool user = Algo::Iter::Contains(
    _users.begin(), _users.end(), UserPredicates::MatchNickname(name));
  bool admin = Algo::Iter::Contains(
    _admins.begin(), _admins.end(), UserPredicates::MatchNickname(name));
  bool wl = Algo::Iter::Contains(
    _whitelist.begin(), _whitelist.end(), UserPredicates::MatchNickname(name));

  return (user << 2) | (admin << 1) | wl;
}

bool
Core::Channel::isUser(const std::string& user_tofind)
{
  return userMatch(user_tofind) & (1 << 2);
}

bool
Core::Channel::isUser(Core::User* user_tofind)
{
  return _users.count(user_tofind);
}

void
Core::Channel::addUser(Core::User* user_toadd)
{
  addUserWhitelist(user_toadd);
  if (!isUser(user_toadd)) {
    _users.insert(user_toadd);
  }
}

void
Core::Channel::removeUser(Core::User* user_toremove)
{
  if (isUser(user_toremove)) {
    _users.erase(user_toremove);
  }
}

bool
Core::Channel::isUserWhitelist(const std::string& user_tofind)
{
  return userMatch(user_tofind) & 1;
}

bool
Core::Channel::isUserWhitelist(Core::User* user_tofind)
{
  return _whitelist.count(user_tofind);
}

void
Core::Channel::addUserWhitelist(Core::User* user_toadd)
{
  if (!isUserWhitelist(user_toadd)) {
    _whitelist.insert(user_toadd);
  }
}

void
Core::Channel::removeUserWhitelist(Core::User* user_toremove)
{
  if (isUserWhitelist(user_toremove)) {
    _whitelist.erase(user_toremove);
  }
}

bool
Core::Channel::isAdmin(const std::string& admin_tofind)
{
  return userMatch(admin_tofind) & (1 << 1);
}

bool
Core::Channel::isAdmin(Core::User* admin_tofind)
{
  return _admins.count(admin_tofind);
}

void
Core::Channel::addAdmin(Core::User* admin_toadd)
{
  addUser(admin_toadd);
  if (!isAdmin(admin_toadd)) {
    _admins.insert(admin_toadd);
  }
}

void
Core::Channel::removeAdmin(Core::User* admin_toadd)
{
  if (isAdmin(admin_toadd)) {
    _admins.erase(admin_toadd);
  }
}

const std::string&
Core::Channel::getTopic()
{
  return _topic;
}

void
Core::Channel::setTopic(const std::string& NewTopic)
{
  _topic = NewTopic;
}

bool
Core::Channel::tryPassword(const std::string& PasswordToTest)
{
  return (_password == PasswordToTest);
}

void
Core::Channel::setPassword(const std::string& NewPassword)
{
  _password = NewPassword;
  setIsPasswordProtected(true);
}

size_t
Core::Channel::getUserLimit()
{
  return _userLimit;
}

void
Core::Channel::setUserLimit(size_t NewUserLimit)
{
  _userLimit = NewUserLimit;
}

size_t
Core::Channel::getUserCount()
{
  return _users.size();
}

bool
Core::Channel::getIsInviteOnly()
{
  return _isInviteOnly;
}

void
Core::Channel::setIsInviteOnly(bool state)
{
  _isInviteOnly = state;
}

bool
Core::Channel::getIsTopicModifiable()
{
  return _isTopicModifiable;
}

void
Core::Channel::setIsTopicModifiable(bool state)
{
  _isTopicModifiable = state;
}

bool
Core::Channel::getIsUserLimited()
{
  return _isUserLimited;
}

void
Core::Channel::setIsUserLimited(bool state)
{
  _isInviteOnly = state;
}

bool
Core::Channel::getIsPasswordProtected()
{
  return _isPasswordProtected;
}

void
Core::Channel::setIsPasswordProtected(bool state)
{
  _isPasswordProtected = state;
}

/* Command calls */

void
Core::Channel::joinUser(Core::User* joining_user)
{
  if (getIsPasswordProtected()) {
    std::cout << "Channel is password protected" << std::endl;
    return;
  }
  if (getIsInviteOnly() && !isUserWhitelist(joining_user)) {
    std::cout << "Channel is invite only and user is not whitelisted"
              << std::endl;
    return;
  }
  if (getIsUserLimited() && getUserCount() >= getUserLimit()) {
    std::cout << "Channel full" << std::endl;
    return;
  }
  addUserWhitelist(joining_user);
  addUser(joining_user);
}

void
Core::Channel::joinUser(Core::User* joining_user, const std::string& entered_password)
{
  if (getIsPasswordProtected() && !tryPassword(entered_password)) {
    std::cout << "Incorrect password" << std::endl;
    return;
  }
  if (getIsInviteOnly() && !isUserWhitelist(joining_user)) {
    std::cout << "Channel is invite only and user is not whitelisted"
              << std::endl;
    return;
  }
  if (getIsUserLimited() && getUserCount() >= getUserLimit()) {
    std::cout << "Channel full" << std::endl;
    return;
  }
  addUserWhitelist(joining_user);
  addUser(joining_user);
}

void
Core::Channel::partUser(Core::User* parting_user)
{
  removeUser(parting_user);
}

void
Core::Channel::inviteUser(Core::User* command_user, Core::User* invited_user)
{
  if (!isAdmin(command_user)) {
    std::cout << "invite command requires admin privileges" << std::endl;
    return;
  }
  if (isUserWhitelist(invited_user)) {
    std::cout << "User allready whitelisted" << std::endl;
    return;
  }
  addUserWhitelist(invited_user);
}

void
Core::Channel::kickUser(Core::User* command_user, Core::User* kicked_user)
{
  if (!isAdmin(command_user)) {
    std::cout << "kick command requires admin privileges" << std::endl;
    return;
  }
  if (!isUser(kicked_user)) {
    std::cout << "User you want to kick is not on the channel" << std::endl;
    return;
  }
  removeUser(kicked_user);
  std::cout << kicked_user << " has been kicked" << std::endl;
}

void
Core::Channel::kickUser(Core::User* command_user,
                  Core::User* kicked_user,
                  const std::string& kick_reason)
{
  kickUser(command_user, kicked_user);
  std::cout << kicked_user << " has been kicked for " << kick_reason
            << std::endl;
}

void
Core::Channel::activatePassword(const std::string& password_given)
{
  setPassword(password_given);
  setIsPasswordProtected(true);
}

void
Core::Channel::modifyTopic(Core::User* command_user, const std::string& new_topic)
{
  if (!getIsTopicModifiable() && isAdmin(command_user)) {
    std::cout << "topic is protected and requires admin privileges"
              << std::endl;
    return;
  }
  setTopic(new_topic);
}

void
Core::Channel::Broadcast(const std::string& message, Core::User* except)
{
  for (Users::iterator it = _users.begin(); it != _users.end(); ++it)
    if (*it != except)
      (*it)->SetOutgoingBuffer(message);
}

const Users&
Core::Channel::GetUsers(void)
{
  return _users;
}
