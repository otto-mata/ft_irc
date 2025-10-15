#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <stdint.h>

#include "User.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>

#define DEFAULT_NAME "unamed_channel"
#define DEFAULT_TOPIC ""
#define DEFAULT_PASSWORD ""
#define DEFAULT_USER_LIMIT_COUNT 42
#define DEFAULT_IS_INVITE_ONLY false
#define DEFAULT_IS_TOPIC_MODIFIABLE true
#define DEFAULT_IS_USER_LIMITED false
#define DEFAULT_IS_PASSWORD_PROTECTED false

typedef std::set<Core::User *> Users;

namespace Core {
    class Channel {
    private:
        std::string _name;
        Users _users;
        Users _whitelist;
        Users _admins;
        Core::User *_owner;
        std::string _topic;
        std::string _password;
        size_t _userLimit;
        bool _isInviteOnly;
        bool _isTopicModifiable;
        bool _isUserLimited;
        bool _isPasswordProtected;

        uint8_t userMatch(const std::string &name);

    public:
        Channel();

        Channel(const std::string &channel_name);

        ~Channel();

        /* Basic class settings */

        const std::string &GetName();

        int SetOwner(Core::User *Owner);

        Core::User *GetOwner();

        bool IsUser(const std::string &user_tofind);

        bool IsUser(Core::User *user_tofind);

        void AddUser(Core::User *user_toadd);

        void RemoveUser(Core::User *user_toremove);

        bool IsUserInWhitelist(const std::string &user_tofind);

        bool IsUserInWhitelist(Core::User *user_tofind);

        void AddUserToWhitelist(Core::User *user_toadd);

        void RemoveUserFromWhitelist(Core::User *user_toremove);

        bool IsAdmin(const std::string &admin_tofind);

        bool IsAdmin(Core::User *admin_tofind);

        void AddAdmin(Core::User *admin_toadd);

        void RemoveAdmin(Core::User *admin_toremove);

        const std::string &GetTopic();

        void SetTopic(const std::string &NewTopic);

        bool TryPassword(const std::string &PasswordToTest);

        void SetPassword(const std::string &NewPassword);

        size_t GetUserLimit();

        void SetUserLimit(size_t NewUserLimit);

        size_t GetUserCount();

        bool IsInviteOnly();

        void SetInviteOnly(bool state);

        bool IsTopicModifiable();

        void SetTopicModifiable(bool state);

        bool IsUserLimited();

        void SetUserLimited(bool state);

        bool IsPasswordProtected();

        void SetPasswordProtected(bool state);

        void Broadcast(const std::string &m, Core::User *except = 0);

        const Users &GetUsers();

        std::string GetChannelModes();
    };
} // namespace Core
#endif
