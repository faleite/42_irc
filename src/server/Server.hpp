#ifndef SERVER_HPP
#define SERVER_HPP

#include "../channel/Channel.hpp"
#include "../client/Client.hpp"
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
		
  		void sendWelcomeMessage(Client newClient);
		void createSocket();
		void acceptClient();
		void initServer();
		void closeFds();
		void cleanClient(int fd);
		std::string getMessage(int fd);
		void handleMessage(int fd);
		void stop(); 
		static void closeSignal(int sig);
		void registerSignalHandler();
		// Create channels
  		void createChannel(std::string const &name);
 		bool findChannel(std::string const &channelName);

		std::string const &getPass() const;
		bool checkAuthenticator(Client &client, std::string &command);
		int  parseHandler(Client &client, std::string &message);
};

#endif // SERVER_HPP