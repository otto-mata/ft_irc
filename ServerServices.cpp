#include "ChannelPredicates.hpp"
#include "CustomAlgo.hpp"
#include "Server.hpp"
#include "UserPredicates.hpp"

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

bool
Server::MatchUserByNickname(const std::string& Nickname)
{
  return Algo::Iter::Contains(
    users.begin(), users.end(), UserPredicates::MatchNicknameInMap(Nickname));
}

bool
Server::MatchChannelByName(const std::string& Name)
{
  return Algo::Iter::Contains(
    channels.begin(), channels.end(), ChannelPredicates::MatchNameInMap(Name));
}

Channel*
Server::CreateChannel(const std::string& Name)
{
  if (MatchChannelByName(Name))
    return 0;
  Channel* newChannel = new Channel(Name);
  if (!newChannel)
    return 0;
  channels[Name] = newChannel;
  return newChannel;
}

int
Server::RemoveChannel(Channel* Chan)
{
  if (!Chan)
    return (1);
  if (!MatchChannelByName(Chan->getName()))
    return (2);
  channels.erase(Chan->getName());
  delete Chan;
  return 0;
}
