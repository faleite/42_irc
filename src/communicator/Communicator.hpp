#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <iostream>
#include <string.h>
#include <set>

class Communicator {
public:
  Communicator(){};
  virtual void const sendMessage() = 0;
  virtual void const receiveMessage() = 0;
  virtual void const createConnection() = 0;
  virtual void const closeConnection() = 0;
  ~Communicator(){};
};
#endif