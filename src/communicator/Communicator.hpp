#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <cstring> // for the memeset initialiation.
#include <iostream>
#include <netinet/in.h> // sockaddr_in structure.
#include <poll.h>
#include <set>
#include <string.h>
#include <sys/socket.h> // socked funtions like create, bind, listen.
#include <unistd.h>     // for the close file function.
class Communicator {
public:
  Communicator(){};
  virtual void const sendMessage(std::string const &_message) = 0;
  std::string virtual receiveMessage() = 0;
  virtual void const createConnection() = 0;
  virtual void const closeConnection() = 0;
  ~Communicator(){};
};
#endif