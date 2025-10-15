#include <Channel.hpp>
#include <CustomAlgo.hpp>
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
  _owner = 0;
}

Core::Channel::~Channel() {}

const std::string&
Core::Channel::GetName()
{
  return _name;
}

int
Core::Channel::SetOwner(Core::User* Owner)
{
  if (_owner != 0)
    return 0;
  _owner = Owner;
  AddAdmin(Owner);
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
Core::Channel::IsUser(const std::string& user_tofind)
{
  return userMatch(user_tofind) & (1 << 2);
}

bool
Core::Channel::IsUser(Core::User* user_tofind)
{
  return _users.count(user_tofind);
}

void
Core::Channel::AddUser(Core::User* user_toadd)
{
  AddUserToWhitelist(user_toadd);
  if (!IsUser(user_toadd)) {
    _users.insert(user_toadd);
    user_toadd->addChannel(this, _name);
  }
}

void
Core::Channel::RemoveUser(Core::User* user_toremove)
{
  if (IsUser(user_toremove)) {
	  _users.erase(user_toremove);
    user_toremove->removeChannnel(_name);
  }
}

bool
Core::Channel::IsUserInWhitelist(const std::string& user_tofind)
{
  return userMatch(user_tofind) & 1;
}

bool
Core::Channel::IsUserInWhitelist(Core::User* user_tofind)
{
  return _whitelist.count(user_tofind);
}

void
Core::Channel::AddUserToWhitelist(Core::User* user_toadd)
{
  if (!IsUserInWhitelist(user_toadd)) {
    _whitelist.insert(user_toadd);
  }
}

void
Core::Channel::RemoveUserFromWhitelist(Core::User* user_toremove)
{
  if (IsUserInWhitelist(user_toremove)) {
    _whitelist.erase(user_toremove);
  }
}

bool
Core::Channel::IsAdmin(const std::string& admin_tofind)
{
  return userMatch(admin_tofind) & (1 << 1);
}

bool
Core::Channel::IsAdmin(Core::User* admin_tofind)
{
  return _admins.count(admin_tofind);
}

void
Core::Channel::AddAdmin(Core::User* admin_toadd)
{
  AddUser(admin_toadd);
  if (!IsAdmin(admin_toadd)) {
    _admins.insert(admin_toadd);
  }
}

void
Core::Channel::RemoveAdmin(Core::User* admin_toadd)
{
  if (IsAdmin(admin_toadd)) {
    _admins.erase(admin_toadd);
  }
}

const std::string&
Core::Channel::GetTopic()
{
  return _topic;
}

void
Core::Channel::SetTopic(const std::string& NewTopic)
{
  _topic = NewTopic;
}

bool
Core::Channel::TryPassword(const std::string& PasswordToTest)
{
  return (_password == PasswordToTest);
}

void
Core::Channel::SetPassword(const std::string& NewPassword)
{
  _password = NewPassword;
  SetPasswordProtected(true);
}

size_t
Core::Channel::GetUserLimit()
{
  return _userLimit;
}

void
Core::Channel::SetUserLimit(size_t NewUserLimit)
{
  _userLimit = NewUserLimit;
}

size_t
Core::Channel::GetUserCount()
{
  return _users.size();
}

bool
Core::Channel::IsInviteOnly()
{
  return _isInviteOnly;
}

void
Core::Channel::SetInviteOnly(bool state)
{
  _isInviteOnly = state;
}

bool
Core::Channel::IsTopicModifiable()
{
  return _isTopicModifiable;
}

void
Core::Channel::SetTopicModifiable(bool state)
{
  _isTopicModifiable = state;
}

bool
Core::Channel::IsUserLimited()
{
  return _isUserLimited;
}

void
Core::Channel::SetUserLimited(bool state)
{
  _isInviteOnly = state;
}

bool
Core::Channel::IsPasswordProtected()
{
  return _isPasswordProtected;
}

void
Core::Channel::SetPasswordProtected(bool state)
{
  _isPasswordProtected = state;
}

void
Core::Channel::Broadcast(const std::string& message, Core::User* except)
{
  for (Users::iterator it = _users.begin(); it != _users.end(); ++it)
    if (*it != except)
      (*it)->AppendToOutgoingBuffer(message);
}

const Users&
Core::Channel::GetUsers(void)
{
  return _users;
}
