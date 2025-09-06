#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include "Channel.hpp"
# include "Server.hpp"

class Command
{
	private:
		std::string _user;
		std::string _command;
		std::string _channel_name;
		Channel* _channel;
		std::set<std::string> _params;

	public:
		Command(std::string command_line, std::string command_user, Server* server);
		~Command();
	
};

#endif