#include "Server.hpp"

Server::Server()
{

}

Server::~Server() {}

void Server::addChannel(std::string channel_name)
{
	_channels.insert(std::make_pair(channel_name, Channel(channel_name)));
}

void Server::addUser(std::string user_name)
{
	_users.insert(std::make_pair(user_name, user_name));
}

bool Server::isChannel(std::string channel_name)
{
	return _channels.find(channel_name) != _channels.end();
}

bool Server::isUser(std::string user_name)
{
	return _users.find(user_name) != _users.end();
}

Channel* Server::getChannel(std::string channel_name)
{
	return &_channels[channel_name];
}
