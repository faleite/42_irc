/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/09 22:20:21 by faaraujo         ###   ########.fr       */
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

class Server
{
	private:
		int _port;
		int _sockfd;
		// std::vector<Client> clients;
		// std::vector<struct pollfd> fds;
		// Server(const Server &copyObj);
		// Server &operator=(const Server &assignCopy);
	public:
		Server(std::string port);
		Server();	
		~Server();	
		
		int &getSocketfd();
		void socketCreate();
};

Server::Server() {}

Server::Server(std::string port)
{
	this->_port = std::atoi(port.c_str());
	socketCreate();
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
	// if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
	// 	throw std::runtime_error("Can't set non_blocking");
	
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; // Aceita conexões em qualquer interface de rede
	servAddr.sin_port = htons(_port);
	
	if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error("Can't be bind to IP/port");
	if (listen(_sockfd, SOMAXCONN) == -1)
		throw std::runtime_error("Can't listen");
}

int &Server::getSocketfd() { return (this->_sockfd); }

#endif // SERVER_HPP