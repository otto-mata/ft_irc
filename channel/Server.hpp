#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>

class Channel;

class Server
{
	private:
		std::map<std::string, Channel> _channels;
		std::map<std::string, std::string> _users;

	public:
		Server();
		~Server();

		isChannel(std::string _channel_name);
	
};

#endif