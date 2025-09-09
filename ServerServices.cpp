#include "ChannelPredicates.hpp"
#include "CustomAlgo.hpp"
#include "Server.hpp"
#include "UserPredicates.hpp"

Core::User*
Core::Server::FindUserByNickname(const std::string& Nickname)
{
  return Algo::Iter::FindFirst<UserMap::iterator,
                               UserPredicates::FindByNicknameInMap,
                               User>(
    users.begin(), users.end(), UserPredicates::FindByNicknameInMap(Nickname));
}

Core::Channel*
Core::Server::FindChannelByName(const std::string& Name)
{
  return Algo::Iter::FindFirst<ChannelMap::iterator,
                               ChannelPredicates::FindByNameInMap,
                               Channel>(
    channels.begin(), channels.end(), ChannelPredicates::FindByNameInMap(Name));
}

bool
Core::Server::MatchUserByNickname(const std::string& Nickname)
{
  return Algo::Iter::Contains(
    users.begin(), users.end(), UserPredicates::MatchNicknameInMap(Nickname));
}

bool
Core::Server::MatchChannelByName(const std::string& Name)
{
  return Algo::Iter::Contains(
    channels.begin(), channels.end(), ChannelPredicates::MatchNameInMap(Name));
}

bool
Core::Server::TryPassword(const std::string& attempt)
{
  return attempt == password;
}

void
Core::Server::RemoveUserFromServer(Core::User* user)
{
  for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
       ++it) {
    if (it->second->isUser(user))
      it->second->removeUser(user);
  }
  user->MarkForDeletion();
}

const std::string&
Core::Server::Hostname(void)
{
  return hostName;
}

void
Core::Server::StopServer(void)
{
  mustStop = true;
}

Core::Channel*
Core::Server::CreateChannel(const std::string& Name)
{
  size_t pos = 0;
  if (Name.find('#') == 0)
    pos++;
  std::string noHashtagName = Name.substr(pos);
  if (MatchChannelByName(noHashtagName))
    return 0;
  Core::Channel* newChannel = new Channel(noHashtagName);
  if (!newChannel)
    return 0;
  channels[noHashtagName] = newChannel;
  return newChannel;
}

int
Core::Server::RemoveChannel(Core::Channel* Chan)
{
  if (!Chan)
    return (1);
  if (!MatchChannelByName(Chan->getName()))
    return (2);
  channels.erase(Chan->getName());
  delete Chan;
  return 0;
}
