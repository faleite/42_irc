#include "Server.hpp"
#include <cstdlib>
#include <iostream>

//_______________________________________: Constructor

Server::Server(int const &port, std::string pass) : _port(port), _pass(pass) {
  this->_sockfd = -1;
  createSocket();
  // initServer();
}

Server::Server() : _sockfd(-1), _port(0), _pass("") {}

Server::Server(const Server &copyObj) { *this = copyObj; }

Server &Server::operator=(const Server &assignCopy) {
  if (this != &assignCopy) {
    this->_port = assignCopy._port;
    this->_pass = assignCopy._pass;
    this->_pfds = assignCopy._pfds;
    this->_sockfd = assignCopy._sockfd;
    this->_signal = assignCopy._signal;
    this->instance = assignCopy.instance;
  }
  return *this;
}

//_______________________________________: Destructor

Server::~Server() {}

Server *Server::instance = NULL;

void Server ::stop() { this->_signal = false; }

void Server::closeSignal(int sig) {
  if (instance == NULL) {
    std::cout << "No server to Shut Down" << std::endl;
    return;
  }
  if (sig == SIGINT || sig == SIGQUIT) {
    instance->closeFds();
    instance->stop();
  }
}
void Server::registerSignalHandler() {
  instance = this;
  signal(SIGINT, closeSignal);
  signal(SIGQUIT, closeSignal);
}

void Server::createSocket() {
  this->_signal = true;
  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd == -1)
    throw std::runtime_error("Can't create a socket");
  int optval = 1;
  if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
      -1)
    throw std::runtime_error("Can't set socket options");
  if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error("Can't set non_blocking");

  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr =
      INADDR_ANY; // Accepts connections on any network interface
  servAddr.sin_port = htons(_port);

  if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    throw std::runtime_error("Can't be bind to IP/port");
  if (listen(_sockfd, SOMAXCONN) == -1)
    throw std::runtime_error("Can't listen");
  std::cout << "Server Running on Port : " << _port << std::endl;
}

void Server::acceptClient() {
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

  int result = getnameinfo((sockaddr *)&client, cliSize, host, NI_MAXHOST,
                           service, NI_MAXSERV, 0);
  if (result != 0)
    throw std::runtime_error("Can't get hostname");

  Client newClient =
      Client(clifd, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
  _clients.push_back(newClient);

  // TESTING
  sendWelcomeMessage(newClient);
}

void Server::initServer() {
  pollfd pfd = {_sockfd, POLLIN, 0};
  _pfds.push_back(pfd);

  //___________________________________SET CHANNELS
  createChannel("#general");
  createChannel("#news");
  createChannel("#random");

  //___________________________________SET BOT
  Client faleiteBot(-1, "FaleiteLegend");
  Client juanBot(-1, "MasterTinxzYoda");
  juanBot.setIsBot(true);
  faleiteBot.setIsBot(true);
  juanBot.setOperator(true);
  faleiteBot.setOperator(true);
  _clients.push_back(juanBot);
  _clients.push_back(faleiteBot);
  std::cout << "Bot :::: " << juanBot.getName() << std::endl;
  std::cout << "Bot :::: " << faleiteBot.getName() << std::endl;

  //___________________________________SET CHANNELS BOTS.
  _channels["#general"].joinChannel(&juanBot, "");
  _channels["#random"].joinChannel(&juanBot, "");
  _channels["#news"].joinChannel(&juanBot, "");

  _channels["#general"].joinChannel(&faleiteBot, "");
  _channels["#random"].joinChannel(&faleiteBot, "");
  _channels["#news"].joinChannel(&faleiteBot, "");

  while (this->_signal) // siganls
  {
    // negative timeout waits forever
    int events = poll(_pfds.data(), _pfds.size(), -1);
    if (events == -1) {
      std::cout << "\nStop\n";
      close(pfd.fd);
      break;
    }
    for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
         it++) {
      if (it->revents & POLLIN) {
        if (it->fd == _sockfd) {
          acceptClient();
          break;
        }
        handleMessage(it->fd);
      }
      if (it->revents & POLLHUP) {
        break;
      }
    }
  }
}

std::string Server::getMessage(int fd) {
  char buffer[1024];
  std::string message;
  int bytesRecv = recv(fd, buffer, 1024, 0);
  if (bytesRecv == -1)
    throw std::runtime_error("");
  else if (bytesRecv > 510) // check in RFC about close connection here
    throw std::runtime_error(
        "Message exceeds 512 bytes limit. Closing connection.");
  else
    message = std::string(buffer, bytesRecv);
  return (message);
}

void Server::closeFds() {
  try {
    for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
         it++)
      close(it->fd);

  } catch (const std::exception &e) {
    throw std::runtime_error("Error delete poll");
  }
}

void Server::handleMessage(int fd) {
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
  // parseHandler(_clients[0], message);
  std::cout << message;
}

void Server::createChannel(std::string const &name) {
  _channels[name] = Channel(name);
}

bool Server::findChannel(std::string const &channelName) {
  std::map<std::string, Channel>::iterator it;
  it = _channels.find(channelName);
  return (it != _channels.end() ? true : false);
}

// #___________________________________________UTILIIES.

void Server::sendWelcomeMessage(Client newClient) {
  std::ostringstream oss;
  oss << "////////////////////////////////////////////////////////////////////"
         "\n"
      << "//                                                                "
         "//\n"
      << "//                              WELCOME                           "
         "//\n"
      << "//                                TO                              "
         "//\n"
      << "//                            /// SERVER ///                      "
         "//\n"
      << "//                                                                "
         "//\n"
      << "////////////////////////////////////////////////////////////////////"
         "\n"
      << "\n"
      << "Hello, " << newClient.getName() << "!\n"
      << "\n"
      << "Welcome to the **best IRC Server** around. We're thrilled to have "
         "you join our community!\n"
      << "\n"
      << "To get started, please set your name and nickname:\n"
      << "- Use `USER <YourName> <ip> <host> <surname>` to set your real "
         "data.\n"
      << "- Use `NICK <YourNickname>` to choose a nickname.\n"
      << "\n"
      << "Once you're all set, feel free to jump into our channels:\n"
      << "- **#general**: For general discussion and announcements.\n"
      << "- **#help**: For support and troubleshooting.\n"
      << "- **#random**: For casual conversation and fun.\n"
      << "\n"
      << "Enjoy your stay and reach out if you need any assistance. Let’s make "
         "this a great place together!\n"
      << "\n"
      << "Best,\n"
      << "The 42_IRC Team\n";

  std::string welcomeMessage = oss.str();
  newClient.sendMessage(welcomeMessage);
}
