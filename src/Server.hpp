/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/14 20:54:02 by faaraujo         ###   ########.fr       */
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
#include <cstdlib>

class Server
{
	private:
		int _port;
		// int _pass;
		int _sockfd;
		// std::vector<Client> clients;
		std::vector<struct pollfd> _pfds;
		// Server(const Server &copyObj);
		// Server &operator=(const Server &assignCopy);
	public:
		Server(std::string port); // STRING PASS HOW SECOND PARAM
		Server(); // how private?
		~Server();	
		
		int getSocketfd();
		void socketCreate();
		void acceptClient();
		void initServer();
		std::string getMessage(int fd);
		// Create Method for close all fd

};

Server::Server() {}

Server::Server(std::string port)
{
	this->_port = std::atoi(port.c_str());
	socketCreate();
	initServer();
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
	{
		close(_sockfd);
		throw std::runtime_error("Can't create a socket");
	}
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

	int clifd = accept(_sockfd, (sockaddr *)&client, &cliSize);
	if (clifd == -1)
		throw std::runtime_error("Problem with client connecting");
	
	pollfd pfd = {clifd, POLLIN, 0};
	_pfds.push_back(pfd);
	
	// FOR NOW::::::::::::FOR NOW
	// HERE GET HOST NAME FOR NOW
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr *)&client, cliSize, 
								host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result != 0)
		throw std::runtime_error("Can't get hostname");	
	else
	{
		std::cout << inet_ntoa(client.sin_addr) << " connected on " << ntohs(client.sin_port) << std::endl;
		// inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		// std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
}

void Server::initServer()
{
	pollfd pfd = {_sockfd, POLLIN, 0};
	_pfds.push_back(pfd);

	while(true) // siganls
	{
		// timeout negativo espera para sempre
		if (poll(_pfds.data(), _pfds.size(), -1) == -1)
			throw std::runtime_error("poll error");
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == _sockfd)
				{
					acceptClient();
					break ;
				}
				std::cout << getMessage(it->fd);
				if (it->revents & POLLHUP) // Make signal handle 
				{
					std::cout << "Break ---- " << std::endl;	
					return ;
				}
			}
		}
	}
}

std::string Server::getMessage(int fd)
{
	// limit 512 bytes incluindo (incluindo o comando, prefixo e os separadores (CRLF, "\r\n").  todos os cabeçalhos e espaços)
	char buffer[1024]; 
	std::string message;
	
	int bytesRecv = recv(fd, buffer, 1024, 0);
	if (bytesRecv == - 1) 
		throw std::runtime_error("There was a connection issue");
	else
	{
		std::cout << "LEN: " << bytesRecv << "\n"; // put except for read bytes recive
		message = std::string(buffer, bytesRecv);
	}
	return (message);
}

int Server::getSocketfd() { return (this->_sockfd); }

#endif // SERVER_HPP