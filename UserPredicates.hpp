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
  Core::User* operator()(std::pair<const int, Core::User*>& pair);
};


struct MatchNicknameInMap
{
private:
  std::string _name;

public:
  MatchNicknameInMap(const std::string& name);
  bool operator()(std::pair<const int, Core::User*>& pair);
};

struct MatchNickname
{
private:
  std::string _name;

public:
  MatchNickname(const std::string& name);
  bool operator()(Core::User* user);
};

struct MatchRealname
{
private:
  std::string _name;

public:
  MatchRealname(const std::string& name);
  bool operator()(Core::User* user);
};

struct MatchUsername
{
private:
  std::string _name;

public:
  MatchUsername(const std::string& name);
  bool operator()(Core::User* user);
};

};

#endif
