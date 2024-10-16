#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <iostream>
#include <set>
#include <string.h>
#include <cstring> // for the memeset initialiation.
#include <sys/socket.h> // socked funtions like create, bind, listen.
#include <netinet/in.h> // sockaddr_in structure.
#include <unistd.h> // for the close file function.
#include <poll.h>
class Communicator {
public:
  Communicator(){};
  virtual void const sendMessage(const int &fd) = 0;
  virtual void const receiveMessage(const int &fd);
  virtual void const createConnection() = 0;
  virtual void const closeConnection() = 0;
  ~Communicator(){};
};
#endif