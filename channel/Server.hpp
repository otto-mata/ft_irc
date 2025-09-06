#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include "Channel.hpp"

class Server
{
	private:
		std::map<std::string, Channel> _channels;
		std::map<std::string, std::string> _users;

	public:
		Server();
		~Server();

		void addChannel(std::string channel_name);
		void addUser(std::string user_name);

		bool isChannel(std::string channel_name);
		bool isUser(std::string user_name);

		Channel* getChannel(std::string channel_name);

};

#endif