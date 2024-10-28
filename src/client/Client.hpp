#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include "../communicator/Communicator.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// class Client : public Communicator {
class Client {
protected:
  int _clientSocket;
  std::string _ip;
  int _port;
  std::string _name;
  std::string _nickName;
  bool _isAuthenticated;
  bool _isOperator;
  bool _isBot;
  std::set<std::string> _channels; // structure to keeep track of the channels,
                                   // in and unorder set.
public:
  // Canonical Form.
  Client();
  Client(int _clientSocket, std::string const &name);
  Client(int _clientSoket, std::string ip, int port);
  ~Client();
  Client(const Client &copyObj);
  Client &operator=(const Client &assignCopy);

  // Function Create Client.
  // Virtual Class functions.
  void sendMessage(std::string const &_message) const;
  void createConnection() const {}
  void closeConnection() const {}
  // Getters.
  int getSocket(void) const;
  std::string const &getName(void) const;
  std::string const &getNickName(void) const;
  bool getAuthenticator(void) const;
  bool getIsOperator(void) const;
  std::string const &getIp() const;
  int getPort() const;
  bool getIsBot(void) const;
  // Setters.
  void setName(const std::string _name);
  void setNickName(const std::string _nick);
  void setOperator(bool _isOperator);
  void setAuthenticated(bool _pass);
  void setIsBot(bool isBot);
  // Operating Functions.
  void joinChanel(const std::string &_chanel,
                  const std::string &password) const;
  void leaveChanel(const std::string &_chanel) const;

  // Factory method

  // setters
};

Client *createRandomClient(int clientSocket);

#endif