#ifndef USER_PREDICATES_HPP
#define USER_PREDICATES_HPP
#include "User.hpp"
#include <string>

namespace UserPredicates {

struct FindByNicknameInMap
{
private:
  std::string _name;

public:
  FindByNicknameInMap(const std::string& name);
  User* operator()(std::pair<const int, User *>& pair);
};

struct MatchNickname
{
private:
  std::string _name;

public:
  MatchNickname(const std::string& name);
  bool operator()(User* user);
};

struct MatchRealname
{
private:
  std::string _name;

public:
  MatchRealname(const std::string& name);
  bool operator()(User* user);
};

struct MatchUsername
{
private:
  std::string _name;

public:
  MatchUsername(const std::string& name);
  bool operator()(User* user);
};

};

#endif
