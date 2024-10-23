#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <fcntl.h>
#include <sys/types.h>
#include <poll.h>
#include <cstdlib>
#include <sstream>
#include <signal.h>
#include "../client/Client.hpp"

class Server
{
	private:
		int _sockfd;
		std::string _port;
		std::string _pass;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _pfds;
		bool _signal;
		static Server *instance;
		
	public:
		Server();
		Server(const Server &copyObj);
		Server &operator=(const Server &assignCopy);
		Server(std::string port, std::string pass);
		~Server();	
		
		void createSocket();
		void acceptClient();
		void initServer();
		void closeFds();
		std::string getMessage(int fd);
		void handleMessage(int fd);
		void stop(); 
		static void closeSignal(int sig);
		void registerSignalHandler();

		std::string const &getPass() const;
		bool checkAuthenticator(Client &client, std::string &command);
		int  parseHandler(Client &client, std::string &message);
};

#endif // SERVER_HPP