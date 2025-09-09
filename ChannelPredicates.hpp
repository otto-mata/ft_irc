#ifndef CHANNEL_PREDICATES_HPP
#define CHANNEL_PREDICATES_HPP
#include "Channel.hpp"
#include <string>

namespace ChannelPredicates {

struct FindByNameInMap
{
private:
  std::string _name;

public:
  FindByNameInMap(const std::string& name);
  Core::Channel* operator()(std::pair<const std::string, Core::Channel*>& pair);
};

struct MatchNameInMap
{
private:
  std::string _name;

public:
  MatchNameInMap(const std::string& name);
  bool operator()(std::pair<const std::string, Core::Channel*>& pair);
};
};

#endif
