#include "UserPredicates.hpp"

UserPredicates::FindByNickName::FindByNickName(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::FindByNickName::operator()(User* user)
{
  return _name == user->GetNickname();
}

UserPredicates::FindByUsername::FindByUsername(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::FindByUsername::operator()(User* user)
{
  return _name == user->GetUsername();
}

UserPredicates::FindByRealname::FindByRealname(const std::string& name)
  : _name(name)
{
}

bool
UserPredicates::FindByRealname::operator()(User* user)
{
  return _name == user->GetRealName();
}
