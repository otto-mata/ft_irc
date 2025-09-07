#include "Command.hpp"

MessageCommand::MessageCommand(std::string command_line, std::string command_user, Server* server) : _raw(command_line), _user(command_user), _server(server)
{
	_command.clear();
	_target_name.clear();
	_params.clear();
	_trailing.clear();

	std::string word;
	int word_count = 0;

	int j = 0;
	int i = j;
	while (i < _raw.length())
	{
		if (_raw[i] == ':')
		{
			j = i;
			while (_raw[i + j] != ' ' && _raw[i + j] != '\0' && _raw[i + j] != '\r')
				j++;
			_trailing = _raw.substr(i, j);
			_trailing = _trailing.substr(1);
			break ;
		}
		j = 0;
		while (_raw[i + j] != ' ' && _raw[i + j] != '\0' && _raw[i + j] != '\r')
		{
			j++;
		}
		word = _raw.substr(i, j);
		i += j;
		if (word_count == 0)
		{
			_command = word;
			word_count++;
		}
		else if (word_count == 1)
		{
			_target_name = word;
			word_count++;
		}
		else
		{
			_params.insert(word);
			word_count++;
		}
		i++;
	}


	if (word_count < 2)
	{
		std::cout << "invalid command" << std::endl;
		return ;
	}
	/*std::cout << _user << std::endl;
	std::cout << _command << std::endl;
	std::cout << _target_name << std::endl;
	if (_params.size())
	{
		for (std::set<std::string>::iterator it = _params.begin(); it != _params.end(); ++it)
		{
			std::cout << *it << std::endl;
		}
	}
	else
	{
		std::cout << "no parameters" << std::endl;
	}
	if (_trailing.size())
	{
		std::cout << _trailing << std::endl;
	}
	else
	{
		std::cout << "no trailing" << std::endl;
	}*/
}

MessageCommand::~MessageCommand() {}

void MessageCommand::execute()
{
	// private message
	if (_target_name[0] != '#' && (_command == "MSG" || _command == "PRIVMSG"))
	{
		if (_target_name == _user)
			std::cout << "Can't send a message to yourself" << std::endl;
		else if (!_server->isUser(_target_name))
			std::cout << "User does not exist on server" << std::endl;
		else
			std::cout << "Private msg sent to " + _target_name << std::endl;
		return ;
	}
	if (_target_name.size() < 2 || _target_name[0] != '#')
	{
		std::cout << "Invalid channel name" << std::endl;
		return ;
	}
	_target_name = _target_name.substr(1);
	if (!_server->isChannel(_target_name))
	{
		std::cout << "Channel does not exist" << std::endl;
		return ;
	}
	_channel = _server->getChannel(_target_name);
	// channel message
	if (_command == "MSG" || _command == "PRIVMSG")
		this->execMsg();
	else if (_command == "TOPIC")
		this->execTopic();
	else if (_trailing.length() > 0)
		std::cout << "Invalid command no trialing allowed" << std::endl;
	else if (_command == "KICK")
		this->execKick();
	else if (_command == "INVITE")
		this->execInvite();
	else if (_command == "JOIN")
		this->execJoin();
	else if (_command == "PART")
		this->execpart();
	/*if (_command == "MODE")
		this->execMode();
	if (_command == "NICK")
		this->execNick();*/
}

void MessageCommand::execTopic()
{
	if (_params.size() > 0)
	{
		std::cout << "Topic can't have any params" << std::endl;
		return ;
	}
	if (_trailing.length() > 0)
	{
		_channel->modifyTopic(_user, _trailing);
	}
	else
	{
		std::cout << _target_name << " topic is : " << _channel->getTopic() << std::endl;
	}
}

void MessageCommand::execMsg()
{
	if (_params.size() > 0)
	{
		std::cout << _command + " can't have any params" << std::endl;
		return ;
	}
	if (_trailing.length() > 0)
	{
		if (!_channel->isUser(_user))
		{
			std::cout << _user << " is not a member of " << _target_name << std::endl;
			return ;
		}
		std::cout << "#" + _target_name + ": " << _trailing << std::endl;
	}
	else
	{
		std::cout << "Msg requires a trailing" << std::endl;
	}
}

void MessageCommand::execKick()
{
	if (_params.size() != 1)
	{
		std::cout << "Kick requires only the name of the person to kick as parameter" << std::endl;
		return ;
	}
	std::set<std::string>::iterator it = _params.begin();
	if (_trailing.length() > 0)
	{
		_channel->kickUser(_user, *it, _trailing);
	}
	else
	{
		_channel->kickUser(_user, *it);
	}
}

void MessageCommand::execInvite()
{
	if (_params.size() != 1)
	{
		std::cout << "Invite requires only the name of the person to Invite as parameter" << std::endl;
		return ;
	}
	std::set<std::string>::iterator it = _params.begin();
	_channel->inviteUser(_user, *it);
}

void MessageCommand::execJoin()
{
	if (_params.size() != 0)
	{
		std::cout << "Join requires no parameters" << std::endl;
		return ;
	}
	_channel->joinUser(_user);
}

void MessageCommand::execpart()
{
	if (_params.size() != 0)
	{
		std::cout << "Part requires no parameters" << std::endl;
		return ;
	}
	_channel->partUser(_user);
}
