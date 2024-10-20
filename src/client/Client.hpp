#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include "../communicator/Communicator.hpp"
#include <string>
#include <iostream>
#include <set>

// class Client : public Communicator {
class Client {
private:
  int _clientSocket;
  std::string _ip;
	int _port;
  std::string _name;
  std::string _nickName;
  bool _isAuthenticated;
  bool _isOperator;
  std::set<std::string> _channels; // structure to keeep track of the channels,
                                   // in and unorder set.
public:
  // Canonical Form.
  Client();
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
	int getClientSoket() const;

  // Setters.
  void setName(const std::string _name);
  void setNickName(const std::string _nick);
  void setOperator(bool _isOperator);
  void setAuthenticated(bool _pass);

  // Operating Functions.
  void joinChanel(const std::string &_chanel) const;
  void leaveChanel(const std::string &_chanel) const;

  // Factory method

  // setters

};

Client *createRandomClient(int clientSocket);

#endif