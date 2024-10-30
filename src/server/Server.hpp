#ifndef SERVER_HPP
#define SERVER_HPP

#include "../channel/Channel.hpp"
#include "../client/Client.hpp"
#include "../file/File.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define BUFFER_SIZE 1024
class Server
{
	private:
		int _sockfd;
		int _port;
		std::string _pass;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _pfds;
		static Server *instance;
		bool _signal;
		std::map<std::string, Channel> _channels; // structure to keeep track of the channels,
		
	public:
		Server();
		Server(const Server &copyObj);
		Server &operator=(const Server &assignCopy);
		Server(int const &port, std::string pass);
		~Server();

		// ________________________ CONNECTION HANDLER
		void createSocket();
		void acceptClient();
		void initServer();
		void cleanClient(int fd);
		void closeFds();
		void stop();
		void registerSignalHandler();
		static void closeSignal(int sig);

		// ________________________ MSG HANDLER.
		void brodcastMessage(std::string const &messge);
  		void sendWelcomeMessage(Client newClient);
		std::string getMessage(int fd);
		void handleMessage(int fd);

		// ________________________ COMMANDS HANDLER.
		void pass(Client &client, const std::vector<std::string>&param);
		void nick(Client &client, const std::vector<std::string>&param);
		void user(Client &client, const std::vector<std::string>&param);
		void join(Client &client, const std::vector<std::string>&param);

		typedef void (Server::*CommandFunc)(Client&, const std::vector<std::string>&);
    	std::map<std::string, CommandFunc> commandMap;

		//_________________________ File transfer
		void fileTransfer(int const &clienteFd, std::string const &paht);

		// ________________________ CHANNEL MESSAGE.
		void channelManager(Client *client, std::string &channelName);
		void comunicationManager(Client *client, std::string messge);
  		void createChannel(std::string const &name);
 		bool findChannel(std::string const &channelName);


		// ________________________ AUTHENTICATION.
		std::string const &getPass() const;
		int  parseHandler(Client &client, std::string &message);
};

#endif // SERVER_HPP