#include "UserPredicates.hpp"

UserPredicates::FindByNicknameInMap::FindByNicknameInMap(
  const std::string& name)
  : _name(name)
{
}

User*
UserPredicates::FindByNicknameInMap::operator()(
  std::pair<const int, User*>& pair)
{
  return _name == pair.second->GetNickname() ? pair.second : 0;
}

UserPredicates::MatchNickname::MatchNickname(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::MatchNickname::operator()(User* user)
{
  return _name == user->GetNickname();
}

UserPredicates::MatchUsername::MatchUsername(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::MatchUsername::operator()(User* user)
{
  return _name == user->GetUsername();
}

UserPredicates::MatchRealname::MatchRealname(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::MatchRealname::operator()(User* user)
{
  return _name == user->GetRealName();
}

UserPredicates::MatchNicknameInMap::MatchNicknameInMap(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::MatchNicknameInMap::operator()(
  std::pair<const std::string, User*>& pair)
{
  return _name == pair.second->GetNickname();
}
