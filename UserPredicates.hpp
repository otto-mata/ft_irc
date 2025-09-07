#ifndef USER_PREDICATES_HPP
#define USER_PREDICATES_HPP
#include "User.hpp"
#include <string>

namespace UserPredicates {

struct FindByNickName
{
private:
  std::string _name;

public:
  FindByNickName(const std::string& name);
  bool operator()(User* user);
};

struct FindByRealname
{
private:
  std::string _name;

public:
  FindByRealname(const std::string& name);
  bool operator()(User* user);
};

struct FindByUsername
{
private:
  std::string _name;

public:
  FindByUsername(const std::string& name);
  bool operator()(User* user);
};

};

#endif
