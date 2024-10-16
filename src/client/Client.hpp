#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../communicator/Communicator.hpp"
#include <string>

class Client : public Communicator {
private:
  int _clientSocket;
  std::string _name;
  std::string _nickName;
  bool _isAuthenticated;
  bool _isOperator;
  std::set<std::string> _channels; // structure to keeep track of the channels,
                                   // in and unorder set.
public:
  // Canonical Form.
  Client();
  Client(int _clientSoket, std::string const &name, std::string const &_nick);
  Client(const Client &_other);
  Client &operator=(const Client &_other);

  // Virtual Class functions.
  void const sendMessage(void);
  void const receiveMessage(void);
  void const createConnection(void);
  void const closeConnection(void);

  // Getters.
  int const getSocket(void);
  std::string const getName(void);
  std::string const getNickName(void);
  bool const getAuthenticator(void);
  bool const getIsOperator(void);

  // Setters.
  void setName(const std::string _name);
  void setNickName(const std::string _nick);
  void setOperator(bool _isOperator);
  void setAuthenticated(bool _pass);

  // Operating Functions.
  void const joinChanel(const std::string &_chanel);
  void const leaveChanel(const std::string &_chanel);
  // setters
  ~Client(void);
};
#endif