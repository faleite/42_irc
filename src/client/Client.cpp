#include "Client.hpp"
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

// ________ Constructors ________
Client::Client()
    : _clientSocket(-1), _ip(""), _port(0), _name(""), _nickName(""), _authAttempted(false), 
    _isAuthenticated(false), _isRegistered(false), _isOperator(false), _isBot(false) {
  std::cout << "Default Client Constructor Called" << std::endl; // Tolk
  // about it
}

Client::Client(int clientSocket, std::string const &name)
  : _clientSocket(clientSocket), _ip(""), _port(0), _name(name), _nickName(""), _authAttempted(false), 
  _isAuthenticated(false), _isRegistered(false), _isOperator(false), _isBot(false) {
  std::cout << "Name Client Constructor Called" << std::endl; // Tolk
}
Client::Client(int clientSoket, std::string ip, int port)
  : _clientSocket(clientSoket), _ip(ip), _port(port), _name(""), _nickName(""), _authAttempted(false), 
  _isAuthenticated(false), _isRegistered(false), _isOperator(false), _isBot(false) {
  // std::cout << "Client Socket Constructor Connected" << std::endl; // Tolk
  // about it
}

Client::Client(const Client &copyObj) { *this = copyObj; }
Client &Client::operator=(const Client &assignCopy) {
  if (this != &assignCopy) {
    this->_clientSocket = assignCopy._clientSocket;
    this->_ip = assignCopy._ip;
    this->_port = assignCopy._port;
    this->_name = assignCopy._name;
    this->_nickName = assignCopy._nickName;
    this->_authAttempted = assignCopy._authAttempted;
    this->_isAuthenticated = assignCopy._isAuthenticated;
    this->_isRegistered = assignCopy._isRegistered;
    this->_isOperator = assignCopy._isOperator;
    this->_isBot = assignCopy._isBot;
  }
  return *this;
}

Client::~Client() {
} // { std::cout << "Client Destroyed" << std::endl; }; // Tolk about it

// Getter.

int Client::getSocket(void) const { return (_clientSocket); }
std::string const &Client::getName(void) const { return (_name); }
std::string const &Client::getNickName(void) const { return (_nickName); }
bool Client::getAuthenticator(void) const { return (_isAuthenticated); }
bool Client::getAuthAttempted() const { return _authAttempted; }
bool Client::getRegistered() const { return _isRegistered; }
bool Client::getIsOperator(void) const { return (_isOperator); }
std::string const &Client::getIp() const { return this->_ip; }
int Client::getPort() const { return this->_port; }
bool Client::getIsBot(void) const { return this->_isBot; }
// Setter.

void Client::setName(const std::string _name) { this->_name = _name; }
void Client::setNickName(const std::string _nick) { this->_nickName = _nick; }
void Client::setOperator(const bool _isOperator) {
  this->_isOperator = _isOperator;
}
void Client::setAuthAttempted(bool _attempted) { 
  this->_authAttempted = _attempted; 
}
void Client::setAuthenticated(const bool _pass) {
  this->_isAuthenticated = _pass;
}
void Client::setRegistered(const bool _register) {
  this->_isRegistered = _register;
}

void Client::setIsBot(bool isBot) { this->_isBot = isBot; }

void Client::getMessage(std::string const &_message) const {
  std::string msg = _message + "\r\n"; // Update
  // file modification.
  send(_clientSocket, msg.c_str(), msg.size(), 0); // Update
}

void Client::getFile(int serverSocket, std::string const &outputFilePath) {
  std::ofstream outputFile(outputFilePath.c_str(), std::ios::binary);
  if (!outputFile.is_open()) {
    std::cout << "Error with the output file" << std::endl;
    return;
  }
  char buffer[BUFFER_SIZE];
  while (true) {
    int bytesRead = recv(serverSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
      return;
    }
    outputFile.write(buffer, bytesRead);
    int sendMessage = send(serverSocket, "ACK", 3, 0);
    if (sendMessage == -1) {
      std::cout << "Failed to send the ACK " << std::endl;
      return;
    }
  }
}
void Client::joinChanel(const std::string &_chanel,
                        const std::string &password = "") const {
  std::string command;
  command = "JOIN " + _chanel;
  if (password.empty() == false) {
    command += " " + password;
  }
  getMessage(command);
}
