#include "ChannelPredicates.hpp"

ChannelPredicates::FindByNameInMap::FindByNameInMap(const std::string& name): _name(name) {}

Core::Channel*
ChannelPredicates::FindByNameInMap::operator()(std::pair<const std::string, Core::Channel*>& pair)
{
  return _name == pair.second->getName() ? pair.second : 0;;
}

ChannelPredicates::MatchNameInMap::MatchNameInMap(const std::string& name):_name(name) {}

bool
ChannelPredicates::MatchNameInMap::operator()(std::pair<const std::string, Core::Channel*>& pair)
{
  return _name == pair.second->getName();
}
