/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/18 17:59:23 by faaraujo         ###   ########.fr       */
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
#include <map>
#include <fcntl.h>
#include <sys/types.h>
#include <poll.h>
#include <cstdlib>

class Client
{
	private:
		int _fd;
		std::string _host;
		int _port;
	public:
		Client(int fd, std::string host, int port);
		// Client(int fd);
		Client();
		void sendMessage(std::string const &_message);
		std::string getHost();
		int getPort();
		int getFd();
};


/**
 * TODO
 * - Create Method for close all fds
 */
class Server
{
	private:
		int _sockfd;
		std::string _port;
		std::string _pass;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _pfds;
		Server();
	public:
		Server(std::string port, std::string pass);
		Server(const Server &copyObj);
		Server &operator=(const Server &assignCopy);
		~Server();	
		
		void createSocket();
		void acceptClient();
		void initServer();
		void clientExit(int fd);
		std::string getMessage(int fd);
		void handleMessage(int fd);
};

#endif // SERVER_HPP