#include "Client.hpp"
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// ________ Constructors ________
Client::Client()
    : _clientSocket(-1), _ip(""), _port(0), _name(""), _nickName(""),
      _isAuthenticated(false), _isOperator(false) {
  // std::cout << "Default Client Constructor Called" << std::endl; // Tolk
  // about it
}

Client::Client(int clientSoket, std::string ip, int port)
    : _clientSocket(clientSoket), _ip(ip), _port(port), _name(""),
      _nickName(""), _isAuthenticated(false), _isOperator(false) {
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
    this->_isAuthenticated = assignCopy._isAuthenticated;
    this->_isOperator = assignCopy._isOperator;
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
bool Client::getIsOperator(void) const { return (_isOperator); }
std::string const &Client::getIp() const { return this->_ip; }
int Client::getPort() const { return this->_port; }
int Client::getClientSoket() const { return this->_clientSocket; }

// Setter.

void Client::setName(const std::string _name) { this->_name = _name; }
void Client::setNickName(const std::string _nick) { this->_nickName = _nick; }
void Client::setOperator(const bool _isOperator) {
  this->_isOperator = _isOperator;
}
void Client::setAuthenticated(const bool _pass) {
  this->_isAuthenticated = _pass;
}

void Client::sendMessage(std::string const &_message) const {
  std::string msg = _message + "\r\n";             // Update
  send(_clientSocket, msg.c_str(), msg.size(), 0); // Update
}

void Client::joinChanel(const std::string &_chanel,
                        const std::string &password = "") const {
  std::string command;
  command = "JOIN " + _chanel;
  if (password.empty() == false) {
    command += " " + password;
  }
  sendMessage(command);
}

