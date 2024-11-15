#include "Server.hpp"
#include <cstdlib>
#include <iostream>

//_______________________________________: Constructor

Server::Server(int const &port, std::string pass) : _port(port), _pass(pass) {
  this->_sockfd = -1;
  commandMap[NICK] = &Server::nick;
  commandMap[USER] = &Server::user;
  commandMap[PASS] = &Server::pass;
  commandMap[QUIT] = &Server::quit;
  commandMap[PRIVMSG] = &Server::privmsg;
  commandMap[JOIN] = &Server::join;
  commandMap[LIST] = &Server::list;
  commandMap[MODE] = &Server::mode;
  commandMap[TOPIC] = &Server::topic;
  commandMap[INVITE] = &Server::invite;
  commandMap[KICK] = &Server::kick;
  commandMap[PART] = &Server::part;
  commandMap[JOKE] = &Server::joke;
}

Server::Server() : _sockfd(-1), _port(0), _pass("") {
  commandMap[NICK] = &Server::nick;
  commandMap[USER] = &Server::user;
  commandMap[PASS] = &Server::pass;
  commandMap[QUIT] = &Server::quit;
  commandMap[PRIVMSG] = &Server::privmsg;
  commandMap[JOIN] = &Server::join;
  commandMap[LIST] = &Server::list;
  commandMap[MODE] = &Server::mode;
  commandMap[INVITE] = &Server::invite;
  commandMap[KICK] = &Server::kick;
  commandMap[PART] = &Server::part;
  commandMap[JOKE] = &Server::joke;
}

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

Server::~Server() {
  for (std::vector<Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++) {
    delete *it;
  }
  this->_clients.clear();
}

std::string const &Server::getPass() const { return (_pass); }

// Handle Signals

Server *Server::instance = NULL;

void Server ::stop() { this->_signal = true; }

void Server::closeSignal(int sig) {
  if (instance == NULL) {
    std::cout << "No server to Shut Down" << std::endl;
    return;
  }
  if (sig == SIGINT || sig == SIGQUIT) {
    instance->stop();
  }
}
void Server::registerSignalHandler() {
  instance = this;
  signal(SIGINT, closeSignal);
  signal(SIGQUIT, closeSignal);
}

// Handle Server

void Server::createSocket() {
  this->_signal = false;
  int optval = 1;

  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr =
      INADDR_ANY; // Accepts connections on any network interface
  servAddr.sin_port = htons(_port);

  _sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sockfd == -1)
    throw std::runtime_error("Can't create a socket");
  if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
      -1)
    throw std::runtime_error("Can't set socket options");
  if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error("Can't set non_blocking");

  if (bind(_sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    throw std::runtime_error("Can't be bind to IP/port");
  if (listen(_sockfd, SOMAXCONN) == -1)
    throw std::runtime_error("Can't listen");
  std::cout << "Server Running on Port : " << _port << std::endl;

  pollfd pfd = {_sockfd, POLLIN, 0};
  _pfds.push_back(pfd);
}

void Server::acceptClient() {
  sockaddr_in client;
  socklen_t cliSize = sizeof(client);

  int clifd = accept(_sockfd, (sockaddr *)&client, &cliSize);
  if (clifd == -1)
    throw std::runtime_error("Problem with client connecting");
  if (fcntl(clifd, F_SETFL, O_NONBLOCK) == -1)
    throw std::runtime_error("Can't set non_blocking");

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

  Client *newClient =
      new Client(clifd, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
  _clients.push_back(newClient);

  std::cout << "Client connected :fd: " << clifd
            << " port: " << ntohs(client.sin_port) << std::endl;
  newClient->getMessage(
      "To get started, please set your pass, name and nickname:\n \
   - Use `PASS <ServerPass>` to set the server irc pass.\n \
   - Use `USER <YourName> <ip> <host> <surname>` to set your real data.\n \
   - Use `NICK <YourNickname>` to choose a nickname.\n");
}

void Server::initServer() {
  this->createSocket();

  // //___________________________________SET CHANNELS
  // createChannel("#general");
  // createChannel("#news");
  // createChannel("#random");

  // //___________________________________SET BOT
  Client *faleiteBot = new Client(-1, "FaleiteLegend");
  faleiteBot->setNickName("faleite");
  Client *juanBot = new Client(-1, "MasterTinxzYoda");
  juanBot->setIsBot(true);
  juanBot->setNickName("juan");
  faleiteBot->setIsBot(true);
  juanBot->setOperator(true);
  faleiteBot->setOperator(true);
  _clients.push_back(juanBot);
  _clients.push_back(faleiteBot);
  std::cout << "Bot :::: " << juanBot->getName() << std::endl;
  std::cout << "Bot :::: " << faleiteBot->getName() << std::endl;

  // //___________________________________SET CHANNELS BOTS.
  // _channels["#general"].joinChannel(&juanBot, "");
  // _channels["#random"].joinChannel(&juanBot, "");
  // _channels["#news"].joinChannel(&juanBot, "");

  // _channels["#general"].joinChannel(&faleiteBot, "");
  // _channels["#random"].joinChannel(&faleiteBot, "");
  // _channels["#news"].joinChannel(&faleiteBot, "");

  while (!this->_signal) {
    // negative timeout waits forever
    int events = poll(&_pfds[0], _pfds.size(), -1);
    if (events == -1) {
      std::cout << "\nStop\n";
      close(_pfds[0].fd);
      break;
    }
    for (size_t i = 0; i < _pfds.size(); i++) {
      if (_pfds[i].revents & POLLIN) {
        if (_pfds[i].fd == _sockfd)
          this->acceptClient();
        else
          this->handleMessage(_pfds[i].fd);
      }
    }
  }
  closeFds();
}

void Server::closeFds() {
  for (std::vector<Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++) {
    if ((*it)->getSocket() != -1) {
      std::cout << "Client has been disconnected :fd: " << (*it)->getSocket()
                << std::endl;
      close((*it)->getSocket());
    }
  }
  if (_sockfd != -1) {
    std::cout << "Server has been disconnected :fd: " << _sockfd << std::endl;
    close(_sockfd);
  }
}

void Server::cleanClient(int fd) {
  for (std::vector<Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++) {
    if ((*it)->getSocket() == fd) {
      it = _clients.erase(it);
      break;
    }
  }
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       it++) {
    if (it->fd == fd) {
      it = _pfds.erase(it);
      break;
    }
  }
  _clientBuffers.erase(fd);
}
