#include "UserPredicates.hpp"
#include <CustomAlgo.hpp>

UserPredicates::FindByNicknameInMap::FindByNicknameInMap(
    const std::string &name)
    : _name(name) {
}

Core::User *
UserPredicates::FindByNicknameInMap::operator()(
    std::pair<const int, Core::User *> &pair) {
    return Algo::String::ToLower(_name) == Algo::String::ToLower(pair.second->GetNickname()) ? pair.second : 0;
}

UserPredicates::MatchNickname::MatchNickname(const std::string &name)
    : _name(name) {
}

bool
UserPredicates::MatchNickname::operator()(Core::User *user) {
    return Algo::String::ToLower(_name) == Algo::String::ToLower(user->GetNickname());
}

UserPredicates::MatchUsername::MatchUsername(const std::string &name)
    : _name(name) {
}

bool
UserPredicates::MatchUsername::operator()(Core::User *user) {
    return _name == user->GetUsername();
}

UserPredicates::MatchRealname::MatchRealname(const std::string &name)
    : _name(name) {
}

bool
UserPredicates::MatchRealname::operator()(Core::User *user) {
    return _name == user->GetRealName();
}

UserPredicates::MatchNicknameInMap::MatchNicknameInMap(const std::string &name)
    : _name(name) {
}

bool
UserPredicates::MatchNicknameInMap::operator()(
    std::pair<const int, Core::User *> &pair) {
    return Algo::String::ToLower(_name) == Algo::String::ToLower(pair.second->GetNickname());
}

UserPredicates::FindByUsernameInMap::FindByUsernameInMap(
    const std::string &name) : _name(name) {
}


Core::User *
UserPredicates::FindByUsernameInMap::operator()(
    std::pair<const int, Core::User *> &pair) {
    return _name == pair.second->GetUsername() ? pair.second : 0;
}
