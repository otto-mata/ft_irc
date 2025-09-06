#include "Server.hpp"
#include "Command.hpp"
#include "Channel.hpp"

int main(int argc, char **argv)
{
	Server serv;
	serv.addUser("bob");
	serv.addUser("franck");
	serv.addUser("tap");
	serv.addChannel("home");
	serv.addChannel("park");
	std::string line;
	std::string user = "bob";
	while (std::getline(std::cin, line))
	{
		if (line == "bob")
		{
			user = "bob";
		}
		else if (line == "franck")
		{
			user = "franck";
		}
		else if (line == "who")
		{
			std::cout << user << std::endl;
		}
		else
		{
			Command cmd(line, user, &serv);
			cmd.execute();
		}
	}
}
