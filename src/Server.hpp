/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/10 20:22:58 by faaraujo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <fcntl.h>
#include <sys/types.h>
#include <poll.h>

class Server
{
	private:
		int _port;
		int _sockfd;
		int _cliSocket;
		// std::vector<Client> clients;
		// std::vector<struct pollfd> fds;
		// Server(const Server &copyObj);
		// Server &operator=(const Server &assignCopy);
	public:
		Server(std::string port);
		Server();	
		~Server();	
		
		int getSocketfd();
		int getCliSocket();
		void socketCreate();
		void acceptClient();
};

Server::Server() {}

Server::Server(std::string port)
{
	this->_port = std::atoi(port.c_str());
	socketCreate();
	acceptClient();
}

Server::~Server() {}

/*
 - int socket(int domain, int type, int protocol);
 args: IPv4, TCP, 0 p/ escolha protcol apropriado
 - int setsockopt(int s, int level, int optname, const void *optval,
				  socklen_t optlen);
 - int fcntl(int s, int cmd, long arg); p/ o socket para ser non-blocking
*/
void Server::socketCreate()
{
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::runtime_error("Can't create a socket");
	int optval = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		throw std::runtime_error("Can't set socket options");
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Can't set non_blocking");
	
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões em qualquer interface de rede
	servAddr.sin_port = htons(_port);
	
	if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error("Can't be bind to IP/port");
	if (listen(_sockfd, SOMAXCONN) == -1)
		throw std::runtime_error("Can't listen");
	
	// close _sockfd
}

void Server::acceptClient()
{
	 // Aceitar a chamada
	sockaddr_in client;
	socklen_t cliSize = sizeof(client);

	_cliSocket = accept(getSocketfd(), (sockaddr *)&client, &cliSize);
	if (_cliSocket == -1)
		throw std::runtime_error("Problem with client connecting");

	// struct pollfd pfd[1];
	// pfd[0].fd = _cliSocket;
	// pfd[0].events = POLLIN;
	// pfd[0].revents = 0;
	
	// poll(pfd, 1, 25000);
	
	// get host name com (gethostbyname ou getnameinfo ou outro)

	// char host[NI_MAXHOST];
	// char service[NI_MAXSERV];
	// memset(host, 0, NI_MAXHOST);
	// memset(service, 0, NI_MAXSERV);

	// int result = getnameinfo((sockaddr *)&client, cliSize, 
	// 							host, NI_MAXHOST, service, NI_MAXSERV, 0);
	// if (result != 0)
	// 	throw std::runtime_error("Can't get hostname");

	//	std::cout << host << " connected on " << service << std::endl;
	// else
	// {
	// 	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	// 	std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	// }
}


int Server::getSocketfd() { return (this->_sockfd); }
int Server::getCliSocket() { return (this->_cliSocket); }

#endif // SERVER_HPP