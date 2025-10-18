#include "ChannelPredicates.hpp"
#include "Server.hpp"
#include "UserPredicates.hpp"
#include <CustomAlgo.hpp>
#include <vector>

Core::User *
Core::Server::FindUserByNickname(const std::string &Nickname) {
    return Algo::Iter::FindFirst<UserMap::iterator,
        UserPredicates::FindByNicknameInMap,
        User>(
        users.begin(), users.end(), UserPredicates::FindByNicknameInMap(Nickname));
}

Core::User *
Core::Server::FindUserByUsername(const std::string &Username) {
    return Algo::Iter::FindFirst<UserMap::iterator,
        UserPredicates::FindByUsernameInMap,
        User>(
        users.begin(), users.end(), UserPredicates::FindByUsernameInMap(Username));
}

Core::Channel *
Core::Server::FindChannelByName(const std::string &Name) {
    std::string key = Algo::String::ToLower(Name);
    if (channels.find(key) == channels.end())
        return 0;
    return channels[key];
}

bool
Core::Server::MatchUserByNickname(const std::string &Nickname) {
    return Algo::Iter::Contains(
        users.begin(), users.end(), UserPredicates::MatchNicknameInMap(Nickname));
}

bool
Core::Server::MatchChannelByName(const std::string &Name) {
    std::string key = Algo::String::ToLower(Name);
    return channels.find(key) != channels.end();
}

bool
Core::Server::TryPassword(const std::string &attempt) {
    return attempt == password;
}

bool
Core::Server::IsPasswordProtected(void) {
    return !password.empty();
}

void
Core::Server::RemoveUserFromServer(Core::User *user) {
    std::vector<Core::Channel *> emptyChannels;
    for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
         ++it) {
        if (it->second->IsUser(user)) {
            it->second->RemoveUser(user);
            if (it->second->GetUserCount() == 0) {
                emptyChannels.push_back(it->second);
            }
        }
    }
    for (std::vector<Core::Channel *>::iterator it = emptyChannels.begin();
         it != emptyChannels.end(); ++it) {
        RemoveChannel(*it);
    }
    user->MarkForDeletion();
}

const std::string &
Core::Server::Hostname(void) {
    return hostName;
}

void
Core::Server::StopServer(void) {
    MustStop = true;
}

std::vector<std::string>
Core::Server::GetAllChannelNames(void) {
    std::vector<std::string> v;
    for (Core::ChannelMap::iterator it = channels.begin(); it != channels.end();
         ++it) {
        v.push_back("#" + it->second->GetName());
    }
    return (v);
}

void
Core::Server::LogNicknameChangeForUser(Core::User *user) {
    if (!user)
        return;
    if (nickHistory.find(user->GetNickname()) == nickHistory.end())
        nickHistory[user->GetNickname()] = std::list<Core::User *>();
    nickHistory[user->GetNickname()].push_front(user);
}

Core::User *
Core::Server::GetLatestUserWithNickname(const std::string &nick) {
    if (nickHistory.find(nick) == nickHistory.end())
        return (0);
    return nickHistory[nick].front();
}

Core::Channel *
Core::Server::CreateChannel(const std::string &Name) {
    size_t pos = 0;
    if (Name.find('#') == 0)
        pos++;
    std::string noHashtagName = Name.substr(pos);
    std::string key = Algo::String::ToLower(noHashtagName);
    if (channels.find(key) != channels.end())
        return 0;
    Core::Channel *newChannel = new Channel(noHashtagName);
    if (!newChannel)
        return 0;
    channels[key] = newChannel;
    return newChannel;
}

int
Core::Server::RemoveChannel(Core::Channel *Chan) {
    if (!Chan)
        return (1);
    std::string key = Algo::String::ToLower(Chan->GetName());
    if (channels.find(key) == channels.end())
        return (2);
    channels.erase(key);
    delete Chan;
    return 0;
}

Core::Channel *
Core::Server::GetChannel(const std::string &name) {
    size_t pos = 0;
    if (name.find('#') == 0)
        pos++;
    std::string noHashtagName = name.substr(pos);
    std::string key = Algo::String::ToLower(noHashtagName);
    if (channels.find(key) == channels.end())
        return 0;
    return channels[key];
}
