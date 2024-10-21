#include "Server.hpp"

Server::Server(std::string port, std::string pass): _port(port), _pass(pass)
{
	this->_sockfd = -1;
	createSocket();
	// initServer();
}

Server::Server(): _sockfd(-1), _port(""), _pass("") {}

Server::Server(const Server &copyObj)
{
	*this = copyObj;
}

Server &Server::operator=(const Server &assignCopy)
{
	if (this != &assignCopy)
	{
		this->_port = assignCopy._port;
		this->_pass = assignCopy._pass;
		this->_pfds = assignCopy._pfds;
		this->_sockfd = assignCopy._sockfd;
	}
	return *this;
}

void Server :: stop() { this->_run = false;}
void Server :: start() { this->_run = true;}

Server::~Server() {}

void Server::createSocket()
{
    this->_run = true;
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
	{
		close(_sockfd);
		throw std::runtime_error("Can't create a socket");
	}
	int optval = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		close(_sockfd);
		throw std::runtime_error("Can't set socket options");
	}
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_sockfd);
		throw std::runtime_error("Can't set non_blocking");
	}
	
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY; // Accepts connections on any network interface
	servAddr.sin_port = htons((std::atoi(_port.c_str())));
	
	if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		close(_sockfd);
		throw std::runtime_error("Can't be bind to IP/port");
	}
	if (listen(_sockfd, SOMAXCONN) == -1)
	{
		close(_sockfd);
		throw std::runtime_error("Can't listen");
	}
}

void Server::acceptClient()
{
	sockaddr_in client;
	socklen_t cliSize = sizeof(client);

	int clifd = accept(_sockfd, (sockaddr *)&client, &cliSize);
	if (clifd == -1)
	{
		close(clifd);
		throw std::runtime_error("Problem with client connecting");
	}
	
	pollfd pfd = {clifd, POLLIN, 0};
	_pfds.push_back(pfd);
	
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr *)&client, cliSize, 
								host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result != 0)
		throw std::runtime_error("Can't get hostname");	

	Client newClient = Client(clifd, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	_clients.push_back(newClient);

	// TESTING
	std::ostringstream oss;
	oss << newClient.getPort();
	newClient.sendMessage("The Server say: Hello port " + oss.str());
}

void Server::initServer()
{
	pollfd pfd = {_sockfd, POLLIN, 0};
	_pfds.push_back(pfd);
	
	while(this->_run) // siganls
	{
		// negative timeout waits forever
		int events = poll(_pfds.data(), _pfds.size(), -1);
		if (events == -1)
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
				handleMessage(it->fd);
			}
			if (it->revents & POLLHUP) // Make signal handle 
			{
				std::cerr << "POLLHUP:::::" << std::endl;
				// clientExit(it->fd);
				break ;
			}
		}
	}
}

std::string Server::getMessage(int fd)
{
	char buffer[1024]; 
	std::string message;
	
	int bytesRecv = recv(fd, buffer, 1024, 0);
	if (bytesRecv <= 0)
	{
		// clientExit(fd);
		// cleanClient(fd);
		// throw std::runtime_error("can't read message");
		return ("");
	}
	else if (bytesRecv > 510) // check in RFC about close connection here
	{
		// cleanClient(fd);
		// throw std::runtime_error("Message exceeds 512 bytes limit. Closing connection.");
		// std::cerr << "Message exceeds 512 bytes limit. Closing connection." << std::endl;
        // clientExit(fd);
		return ("");
	}
	else
		message = std::string(buffer, bytesRecv);
	return (message);
}

void Server::clientExit()
{
	try
	{	
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		{
			close(it->fd);
			// if (it->fd == fd)
			// {
			// 	std::cout << "Client has been disconnected" << std::endl;	
			// 	it = _pfds.erase(it);
			// 	close(fd);
			// 	break ;
			// }
			// else
			// {
			// 	++it;
			// }
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error delete poll");
	}
}

void Server::cleanClient(int fd)
{
	try
	{	
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end();)
		{
			if (it->getClientSoket() == fd)
			{
				it = _clients.erase(it);
				break ;
			}
			else
			{
				++it;
			}
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error clean Client");
	}
}

void Server::handleMessage(int fd)
{
	try
	{
		// for (size_t i = 0; i < _clients.size(); ++i)
		// {
		// 	if (_clients[i].getClientSoket() == fd)
		// 	{
		// 		std::cout <<  _clients[i].getIp() << " connected on port " 
		// 		<< _clients[i].getPort() << std::endl;
		// 		break ;
		// 	}
		// }
		std::string message = this->getMessage(fd);
		std::cout << message;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		throw std::runtime_error("error handling message");
	}
	// TESTING
}
