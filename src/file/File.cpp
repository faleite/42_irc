#include "File.hpp"

File::File() : _filePath(""), _chunckSize(0) {
  std::cout << "Default File Constructor called." << std::endl;
}

File::File(std::string const &filePath, int chunkSize)
    : _filePath(filePath), _chunckSize(chunkSize) {
  std::cout << "File created " << std::endl;
}

//____________________________ FILE OPERATION.

bool File::openFile(void) {
  // Open the file in binary mode.
  _inputFileStream.open(_filePath, std::ios::in | std::ios::binary);
  if (_inputFileStream.is_open() == false) {
    std::cout << "Problem with openin the file " << std::endl;
    return false;
  }
  return true;
}

int File::readChunkFile(char *buffer, int bufferSize) {
  if (_inputFileStream.is_open() == false)
    return (0);
  _inputFileStream.read(buffer, bufferSize);
  return (_inputFileStream.gcount());
}

void File::closeFile() {
  if (_inputFileStream.is_open() == true)
    _inputFileStream.close();
}

bool File::sendFileTransfer(const int &clientSocket, char *buffer, int size) {
  int byteSend = send(clientSocket, buffer, size, 0);
  if (byteSend == -1) {
    std::cout << "Problem with the file transfer" << std::endl;
    return (false);
  }
  return (true);
}