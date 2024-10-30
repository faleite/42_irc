#include "File.hpp"
#include <ios>
#include <stdexcept>

File::File() : _filePath("")  {
  std::cout << "Default file constructor called" << std::endl;
}

File::File(std::string const &filePath)
    : _filePath(filePath) {
  std::cout << "File Constructor Set Up" << std::endl;
  openFile();
}

void File::openFile() {
  _inputFileStream.open(_filePath.c_str(), std::ios::in | std::ios::binary);
  if (_inputFileStream.is_open() == false)
    throw(std::runtime_error("Could not open the file"));
}

int File::readChunkFile(char *buffer, int const &bufferSize) {
  if (_inputFileStream.is_open() == false)
    throw(std::runtime_error("File not available"));
  _inputFileStream.read(buffer, bufferSize);
  return (_inputFileStream.gcount());
}
bool File::sendFileTransfer(int const &clientFd, char *buffer, int bufferSize) {
  int byteSend;
  byteSend = send(clientFd, buffer, bufferSize, 0);
  if (byteSend == -1)
    throw(std::runtime_error("Problem with the file transfer"));
  return true;
}

void File::closeFile() {
  if (_inputFileStream.is_open())
    _inputFileStream.close();
}

File::~File() { closeFile(); }