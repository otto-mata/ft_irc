#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <map>
# include <set>

#define DEFAULT_NAME "unamed_channel"
#define DEFAULT_TOPIC "undefined_topic"
#define DEFAULT_PASSWORD "unsafe_password"
#define DEFAULT_USER_LIMIT_COUNT 42
#define DEFAULT_IS_INVITE_ONLY 0
#define DEFAULT_IS_TOPIC_MODIFIABLE 1
#define DEFAULT_IS_USER_LIMITED 0
#define DEFAULT_IS_PASSWORD_PROTECTED 0

class Channel
{
	private:
		std::string _name;
		std::set<std::string> _users;
		std::set<std::string> _whitelist;
		std::set<std::string> _admins;
		std::string _topic;
		std::string _password;
		size_t _userLimit;
		bool _isInviteOnly;
		bool _isTopicModifiable;
		bool _isUserLimited;
		bool _isPasswordProtected;

	public:
		Channel() : _name(DEFAULT_NAME) {}
		Channel(std::string channel_name);
		~Channel();

		/* Basic class settings */

		std::string getName();
		
		bool isUser(std::string user_tofind);
		void addUser(std::string user_toadd);
		void removeUser(std::string user_toremove);

		bool isUserWhitelist(std::string user_tofind);
		void addUserWhitelist(std::string user_toadd);
		void removeUserWhitelist(std::string user_toremove);

		bool isAdmin(std::string admin_tofind);
		void addAdmin(std::string admin_toadd);
		void removeAdmin(std::string admin_toremove);

		std::string getTopic();
		void setTopic(std::string NewTopic);

		bool tryPassword(std::string PasswordToTest);
		void setPassword(std::string NewPassword);

		size_t getUserLimit();
		void setUserLimit(size_t NewUserLimit);
		size_t getUserCount();

		bool getIsInviteOnly();
		void setIsInviteOnly(bool state);

		bool getIsTopicModifiable();
		void setIsTopicModifiable(bool state);

		bool getIsUserLimited();
		void setIsUserLimited(bool state);

		bool getIsPasswordProtected();
		void setIsPasswordProtected(bool state);
	
		/* Command calls */

		void joinUser(std::string joining_user);
		void joinUser(std::string joining_user, std::string entered_password);
		void partUser(std::string parting_user);
		void inviteUser(std::string command_user, std::string invited_user);
		void kickUser(std::string command_user, std::string kicked_user);
		void kickUser(std::string command_user, std::string kicked_user, std::string kick_reason);
		void activatePassword(std::string password_given);
		void modifyTopic(std::string command_user, std::string new_topic);
};

#endif
