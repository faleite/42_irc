#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include "../communicator/Communicator.hpp"
#include <algorithm>
#include <fstream>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <set>

// class Client : public Communicator {
class Client
{
protected:
  int _clientSocket;
  std::string _ip;
  int _port;
  std::string _name;
  std::string _nickName;
  bool _authAttempted;
  bool _isAuthenticated;
  bool _isRegistered;
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
  void getMessage(std::string const &_message) const;
  void createConnection() const {}
  void closeConnection() const {}
  // Getters.
  int getSocket(void) const;
  std::string const &getName(void) const;
  std::string const &getNickName(void) const;
  bool getAuthAttempted() const;
  bool getAuthenticator(void) const;
  bool getRegistered() const;
  bool getIsOperator(void) const;
  void getFile(int serverSocket, std::string const &outputFile);
  std::string const &getIp() const;
  int getPort() const;
  bool getIsBot(void) const;
  // Setters.
  void setName(const std::string _name);
  void setNickName(const std::string _nick);
  void setOperator(bool _isOperator);
  void setAuthAttempted(bool _attempted);
  void setAuthenticated(bool _pass);
  void setIsBot(bool isBot);
  void setRegistered(const bool _register);

  // Operating Functions.
  void joinChanel(const std::string &_chanel,
                  const std::string &password) const;
  void leaveChanel(const std::string &_chanel) const;

  // Factory method

  // setters
};

Client *createRandomClient(int clientSocket);

#endif