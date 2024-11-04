#ifndef FILE_HPP
#define FILE_HPP

#include "../client/Client.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/socket.h>

class File {
private:
  std::string _filePath;
  std::ifstream _inputFileStream;
  std::ifstream _outFileStream;

public:
  //________________ Constructor and Destructor.
  File();
  File(std::string const &filePath);
  ~File();

  //________________ File Operations.
  void openFile();
  void closeFile();

  //_______________ Size of the chunk.
  int readChunkFile(char *buffer, int const &bufferSize);

  //_______________ Control Operation.
  bool sendFileTransfer(int const &clientFd, char *buffer, int bufferSize);
  // TODO
  //  Receiving and Writing File Chunks
};

#endif