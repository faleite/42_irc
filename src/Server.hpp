/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faaraujo <faaraujo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 21:44:42 by faaraujo          #+#    #+#             */
/*   Updated: 2024/10/08 22:10:08 by faaraujo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>

#include <sys/types.h>

class Server
{
	private:
		int _port;
		int _servSocketFd;
		// std::vector<Client> clients;
		std::vector<struct pollfd> fds;
		Server(const Server &copyObj);
		Server &operator=(const Server &assignCopy);
	public:
		Server();
		void socketCreate();
		~Server();	
};

void Server::socketCreate()
{
	
}

#endif // SERVER_HPP