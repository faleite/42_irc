/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/23 17:48:03 by faaraujo         ###   ########.fr       */
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
#include <sstream>
#include "../client/Client.hpp"

#include <signal.h>



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
};

#endif // SERVER_HPP