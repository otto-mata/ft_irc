#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include "Channel.hpp"
# include "Server.hpp"

class MessageCommand
{
	private:
		std::string _raw;
		std::string _user;
		std::string _command;
		std::string _target_name;
		Channel* _channel;
		Server* _server;
		std::set<std::string> _params;
		std::string _trailing;

		void execMsg();
		void execTopic();
		void execKick();
		void execInvite();
		void execJoin();
		void execpart();
		//void execMode();
		//void execNick();

	public:
		MessageCommand(std::string command_line, std::string command_user, Server* server);
		~MessageCommand();
	
		void execute();
};

#endif
