#include "Channel.hpp"

Channel::Channel(std::string channel_name)  : _name(channel_name), _topic(DEFAULT_TOPIC),
_userLimit(DEFAULT_USER_LIMIT_COUNT), _isInviteOnly(DEFAULT_IS_INVITE_ONLY),
_isTopicModifiable(DEFAULT_IS_TOPIC_MODIFIABLE), _isUserLimited(DEFAULT_IS_USER_LIMITED),
_password(DEFAULT_PASSWORD), _isPasswordProtected(DEFAULT_IS_PASSWORD_PROTECTED) {}

Channel::~Channel() {}

std::string Channel::getName()
{
	return _name;
}

bool Channel::isUser(std::string user_tofind)
{
	return _users.find(user_tofind) != _users.end();
}

void Channel::addUser(std::string user_toadd)
{
	if (!isUser(user_toadd))
	{
		_users.insert(user_toadd);
	}
}

void Channel::removeUser(std::string user_toremove)
{
	if (isUser(user_toremove))
	{
		_users.erase(user_toremove);
	}
}

bool Channel::isUserWhitelist(std::string user_tofind)
{
	return _whitelist.find(user_tofind) != _whitelist.end();
}

void Channel::addUserWhitelist(std::string user_toadd)
{
	if (!isUserWhitelist(user_toadd))
	{
		_whitelist.insert(user_toadd);
	}
}

void Channel::removeUserWhitelist(std::string user_toremove)
{
	if (isUserWhitelist(user_toremove))
	{
		_whitelist.erase(user_toremove);
	}
}

bool Channel::isAdmin(std::string admin_tofind)
{
	return _admins.find(admin_tofind) != _admins.end();
}

void Channel::addAdmin(std::string admin_toadd)
{
	if (!isAdmin(admin_toadd))
	{
		_admins.insert(admin_toadd);
	}
}

void Channel::removeAdmin(std::string admin_toadd)
{
	if (isAdmin(admin_toadd))
	{
		_admins.erase(admin_toadd);
	}
}

std::string Channel::getTopic()
{
	return _topic;
}

void Channel::setTopic(std::string NewTopic)
{
	_topic = NewTopic;
}

bool Channel::tryPassword(std::string PasswordToTest)
{
	return (_password == PasswordToTest);
}

void Channel::setPassword(std::string NewPassword)
{
	_password = NewPassword;
}

size_t Channel::getUserLimit()
{
	return _userLimit;
}

void Channel::setUserLimit(size_t NewUserLimit)
{
	_userLimit = NewUserLimit;
}

size_t Channel::getUserCount()
{
	return _users.size();
}

bool Channel::getIsInviteOnly()
{
	return _isInviteOnly;
}

void Channel::setIsInviteOnly(bool state)
{
	_isInviteOnly = state;
}

bool Channel::getIsTopicModifiable()
{
	return _isTopicModifiable;
}

void Channel::setIsTopicModifiable(bool state)
{
	_isTopicModifiable = state;
}

bool Channel::getIsUserLimited()
{
	return _isUserLimited;
}

void Channel::setIsUserLimited(bool state)
{
	_isInviteOnly = state;
}

bool Channel::getIsPasswordProtected()
{
	return _isPasswordProtected;
}

void Channel::setIsPasswordProtected(bool state)
{
	_isPasswordProtected = state;
}

/* Command calls */

void Channel::joinUser(std::string joining_user)
{
	if (getIsPasswordProtected())
	{
		std::cout << "Channel is password protected" << std::endl;
		return ;
	}
	if (getIsInviteOnly() && !isUserWhitelist(joining_user))
	{
		std::cout << "Channel is invite only and user is not whitelisted" << std::endl;
		return ;
	}
	if (getIsUserLimited() && getUserCount() >= getUserLimit())
	{
		std::cout << "Channel full" << std::endl;
		return ;
	}
	addUserWhitelist(joining_user);
	addUser(joining_user);
}

void Channel::joinUser(std::string joining_user, std::string entered_password)
{
	if (getIsPasswordProtected() && !tryPassword(entered_password))
	{
		std::cout << "Incorrect password" << std::endl;
		return ;
	}
	if (getIsInviteOnly() && !isUserWhitelist(joining_user))
	{
		std::cout << "Channel is invite only and user is not whitelisted" << std::endl;
		return ;
	}
	if (getIsUserLimited() && getUserCount() >= getUserLimit())
	{
		std::cout << "Channel full" << std::endl;
		return ;
	}
	addUserWhitelist(joining_user);
	addUser(joining_user);
}

void Channel::partUser(std::string parting_user)
{
	removeUser(parting_user);
}

void Channel::inviteUser(std::string command_user, std::string invited_user)
{
	if (!isAdmin(command_user))
	{
		std::cout << "invite command requires admin privileges" << std::endl;
		return ;
	}
	if (isUserWhitelist(invited_user))
	{
		std::cout << "User allready whitelisted" << std::endl;
		return ;
	}
	addUserWhitelist(invited_user);
}

void Channel::kickUser(std::string command_user, std::string kicked_user)
{
	if (!isAdmin(command_user))
	{
		std::cout << "kick command requires admin privileges" << std::endl;
		return ;
	}
	if (!isUser(kicked_user))
	{
		std::cout << "User you want to kick is not on the channel" << std::endl;
		return ;
	}
	removeUser(kicked_user);
	std::cout << kicked_user << " has been kicked" << std::endl;
}

void Channel::kickUser(std::string command_user, std::string kicked_user, std::string kick_reason)
{
	if (!isAdmin(command_user))
	{
		std::cout << "kick command requires admin privileges" << std::endl;
		return ;
	}
	if (!isUser(kicked_user))
	{
		std::cout << "User you want to kick is not on the channel" << std::endl;
		return ;
	}
	removeUser(kicked_user);
	std::cout << kicked_user << " has been kicked for " << kick_reason << std::endl;
}

void Channel::activatePassword(std::string password_given)
{
	setPassword(password_given);
	setIsPasswordProtected(true);
}

void Channel::modifyTopic(std::string command_user, std::string new_topic)
{
	if (!getIsTopicModifiable() && isAdmin(command_user))
	{
		std::cout << "topic is protected and requires admin privileges" << std::endl;
		return ;
	}
	setTopic(new_topic);
}
