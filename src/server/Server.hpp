/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/16 20:55:18 by faaraujo         ###   ########.fr       */
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
		// std::vector<Client> clients;
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
};

#endif // SERVER_HPP