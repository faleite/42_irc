#include "Client.hpp"
#include <string>

// ________ Constructors ________
Client::Client()
    : _clientSocket(-1), _name("Default"), _nickName("Defaultinxz"),
      _isAuthenticated(false), _isOperator(false) {
  std::cout << "Default Client Constructor Called" << std::endl;
}

Client::Client(int _clientSoket, std::string const &name,
               std::string const &_nick)
    : _clientSocket(_clientSoket), _name(name), _nickName(_nick),
      _isOperator(false), _isAuthenticated(false) {
  std::cout << "Client Socket Constructor Connected" << std::endl;
}

Client::Client(const Client &_other)
    : _clientSocket(_other._clientSocket), _name(_other._name),
      _nickName(_other._nickName), _isAuthenticated(_other._isAuthenticated),
      _isOperator(_other._isOperator), _channels(_other._channels) {
  std::cout << "Copy Constructor Called" << std::endl;
}

Client &Client::operator=(const Client &_other) {
  if (this != &_other) {
    this->_clientSocket = _other._clientSocket;
    this->_name = _other._name;
    this->_nickName = _other._nickName;
    this->_isAuthenticated = _other._isAuthenticated;
    this->_isOperator = _other._isOperator;
    this->_channels = _other._channels;
  }
  return (*this);
}

// Getter.

int const Client::getSocket(void) { return (_clientSocket); }
std::string const Client::getName(void) { return (_name); }
std::string const Client::getNickName(void) { return (_nickName); }
bool const Client::getAuthenticator(void) { return (_isAuthenticated); }
bool const Client::getIsOperator(void) { return (_isOperator); }

// Setter.

void Client::setName(const std::string _name) { this->_name = _name; }
void Client::setNickName(const std::string _nick) { this->_nickName = _nick; }
void Client::setOperator(const bool _isOperator) {
  this->_isOperator = _isOperator;
}
void Client::setAuthenticated(const bool _pass) {
  this->_isAuthenticated = _pass;
}