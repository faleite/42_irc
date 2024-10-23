#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/socket.h>

class File {
private:
  std::string _filePath;
  std::ifstream _inputFileStream;
  std::ifstream _outFileStream;
  int _chunckSize;

public:
  File();
  File(std::string const &filePath, int chunkSize = 1024);
  ~File();

  // File Operations.
  bool openFile();
  int readChunkFile(char *buffer, int bufferSize);
  void closeFile();

  // SendFile
  bool sendFileTransfer(int const &clientSocket, char *buffer, int bufferSize);
  // TODO
  //  Receiving and Writing File Chunks
};

#endif