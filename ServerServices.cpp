#include "CustomAlgo.hpp"
#include "Server.hpp"
#include "UserPredicates.hpp"
#include "ChannelPredicates.hpp"

User*
Server::FindUserByNickname(const std::string& Nickname)
{
  return Algo::Iter::FindFirst<UserMap::iterator,
                               UserPredicates::FindByNicknameInMap,
                               User>(
    users.begin(), users.end(), UserPredicates::FindByNicknameInMap(Nickname));
}

Channel*
Server::FindChannelByName(const std::string& Name)
{
  return Algo::Iter::FindFirst<ChannelMap::iterator,
                               ChannelPredicates::FindByNameInMap,
                               Channel>(
    channels.begin(), channels.end(), ChannelPredicates::FindByNameInMap(Name));
}
