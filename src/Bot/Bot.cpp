#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

static int sock = -1;

void stopbot(__attribute_maybe_unused__ int n)
{
	std::cout << "Stopping bot" << std::endl;
	close(sock);
}

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: " << argv[0] << " <host> <port> <pass>" << std::endl;
		return 1;
	}

	const char *host = argv[1];
	int port = std::atoi(argv[2]);
	std::cout << port << std::endl;

	// Création de la socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		std::cerr << "Socket creation failure" << std::endl;
		return 1;
	}

	sockaddr_in serv;
	std::memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);

	if (inet_pton(AF_INET, host, &serv.sin_addr) <= 0) {
		std::cerr << "IPV4 binary conversion failure" << std::endl;
		close(sock);
		return 1;
	}

	if (connect(sock, (sockaddr*)&serv, sizeof(serv)) < 0) {
		std::cerr << "Connection failure" << std::endl;
		close(sock);
		return 1;
	}

	// Identification
	std::string nick = "NICK bot42\r\n";
	std::string user = "USER bot42 0 * :Simple Bot\r\n";
	if (argc == 4) {
		std::string pass = argv[3];
		pass = "PASS " + pass + "\r\n";
		send(sock, pass.c_str(), pass.size(), 0);
	}
	send(sock, nick.c_str(), nick.size(), 0);
	send(sock, user.c_str(), user.size(), 0);

	// Rejoindre un channel
	std::string join = "JOIN #general\r\n";
	send(sock, join.c_str(), join.size(), 0);

	signal(SIGINT, stopbot);
	char buffer[512];
	while (true) {
		int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
		if (n <= 0)
			break;
		if (sock < 0)
			return (0);
		buffer[n] = '\0';
		std::string msg(buffer);

		std::cout << msg;

		if (msg.find(" 464 ") != std::string::npos) {
			std::cerr << "Erreur : mot de passe incorrect." << std::endl;
			return (close(sock), 1);
		}

		if (msg.find("PING") == 0) {
			std::string pong = "PONG" + msg.substr(4) + "\r\n";
			send(sock, pong.c_str(), pong.size(), 0);
		}

		if (msg.find("!ping") != std::string::npos) {
			std::string reply = "PRIVMSG #general :PONG\r\n";
			send(sock, reply.c_str(), reply.size(), 0);
		}
		if (msg.find("tblochet") != std::string::npos) {
			std::string reply = "PRIVMSG #general :NON C'EST THOMAS!\r\n";
			send(sock, reply.c_str(), reply.size(), 0);
		}
		if (msg.find("!stopbot") != std::string::npos) {
			return (close(sock), 0);
		}
	}

	close(sock);
	return 0;
}
