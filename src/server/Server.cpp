#include "Server.hpp"

Server::Server(std::string port, std::string pass) : _port(port), _pass(pass)
{
	this->_sockfd = -1;
	createSocket();
}

Server::Server() : _sockfd(-1), _port(""), _pass("") {}

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
		this->_signal = assignCopy._signal;
		this->instance = assignCopy.instance;
	}
	return *this;
}

Server::~Server() {}

std::string const &Server::getPass() const
{
	return (_pass);
}

// Handle Signals

Server *Server::instance = NULL;

void Server ::stop() { this->_signal = false; }

void Server::closeSignal(int sig)
{
	if (instance == NULL)
	{
		std::cout << "No server to Shut Down" << std::endl;
        return;
	}
	if (sig == SIGINT || sig == SIGQUIT)
	{
		instance->closeFds();
		instance->stop();
	}
}
void Server::registerSignalHandler()
{
	instance = this;
	signal(SIGINT, closeSignal);
	signal(SIGQUIT, closeSignal);
}

// Handle Server

void Server::createSocket()
{
	this->_signal = true;
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
	servAddr.sin_addr.s_addr = INADDR_ANY; // Accepts connections on any network interface
	servAddr.sin_port = htons((std::atoi(_port.c_str())));

	if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		throw std::runtime_error("Can't be bind to IP/port");
	if (listen(_sockfd, SOMAXCONN) == -1)
		throw std::runtime_error("Can't listen");
}

void Server::acceptClient()
{
	sockaddr_in client;
	socklen_t cliSize = sizeof(client);

	int clifd = accept(_sockfd, (sockaddr *)&client, &cliSize);
	if (clifd == -1)
		throw std::runtime_error("Problem with client connecting");

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

	while (this->_signal) // siganls
	{
		// negative timeout waits forever
		int events = poll(_pfds.data(), _pfds.size(), -1);
		if (events == -1)
		{
			std::cout << "\nStop\n";
			close(pfd.fd);
			break;
		}
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == _sockfd)
				{
					acceptClient();
					break;
				}
				handleMessage(it->fd);
			}
			if (it->revents & POLLHUP)
			{
				break;
			}
		}
	}
}

std::string Server::getMessage(int fd)
{
	char buffer[1024];
	std::string message;
	int bytesRecv = recv(fd, buffer, 1024, 0);
	if (bytesRecv == -1)
		throw std::runtime_error("");
	else if (bytesRecv > 510) // check in RFC about close connection here
		throw std::runtime_error("Message exceeds 512 bytes limit. Closing connection.");
	else
		message = std::string(buffer, bytesRecv);
	return (message);
}

void Server::closeFds()
{
	try
	{
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
			close(it->fd);

	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error delete poll");
	}
}

////////////////////////////////////

bool Server::checkAuthenticator(Client &client, std::string &command) {
  std::istringstream stream(command);
  std::string password;
  stream >> password;
  if (password == getPass()) 
  {
    client.setAuthenticated(true);
    std::cout << "Correct Password: " << client.getAuthenticator() << std::endl;
    return true;
  } 
  else
	client.sendMessage("ERROR :Invalid password");
  return false;
}

/**
 * Utils:
 * std::cout << "Name cannot be empty" << std::endl;
 * std::cout << "Nickname cannot be empty" << std::endl;
 */
int Server::parseHandler(Client &client, std::string &message) 
{
	std::string cmd;
	std::istringstream stream(message);
	stream >> cmd;

    if (cmd == "USER") 
	{
      std::string name;
      stream >> name;
      if (!name.empty()) 
        client.setName(name);
    } 
	if (cmd == "NICK") 
	{
      std::string nickName;
      stream >> nickName;
      if (!nickName.empty())
        client.setNickName(nickName);
    }
  	if (cmd == "PASS")
	{
    	  std::string password;
    	  stream >> password;
		  if (!password.empty())
	    	  client.setAuthenticated(checkAuthenticator(client, password));
  	}
	return 0;
}

//////////////////////////////////////

void Server::handleMessage(int fd)
{
	std::string message = this->getMessage(fd);
	parseHandler(_clients[0], message);
	std::cout << message;
}