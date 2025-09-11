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

Core::User*
Core::Server::FindUserByUsername(const std::string& Username)
{
  return Algo::Iter::FindFirst<UserMap::iterator,
                               UserPredicates::FindByUsernameInMap,
                               User>(
    users.begin(), users.end(), UserPredicates::FindByUsernameInMap(Username));
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

bool Core::Server::IsPasswordProtected(void)
{
  return !password.empty();
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
  MustStop = true;
}

std::vector<std::string>
Core::Server::GetAllChannelNames(void)
{
  std::vector<std::string> v;
  for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
       ++it) {
    v.push_back(it->first);
  }
  return (v);
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

Core::Channel*
Core::Server::GetChannel(const std::string& name)
{
  return channels[name];
}
