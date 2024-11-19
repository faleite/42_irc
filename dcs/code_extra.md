# CODE EXTRA

## File
### hpp
```cpp
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
```

### cpp 
```cpp
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
```

## SEND MESSAGE (MSG)
```cpp
//________________________________SEND MESSAGE IN THE CHANNEL

void Server::msg(Client &client, const std::string &cmd,
                 const std::vector<std::string> &param)
{
  (void)cmd;
  std::string mess = client.getName() + " : ";
  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i)
  {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();
  if (findChannel(param[0]))
  {
    _channels[param[0]].isOnChannel(client.getNickName())
        ? _channels[param[0]].broadcastMessage(mess, &client)
        : client.getMessage("You are not part of this channel");
  }
  else
    client.getMessage("Channel not found");
}
```

## JOIN TO A CHANNEL
```cpp
void Server::join(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  (void)cmd;
  try
  {
    std::map<std::string, Channel>::iterator it = _channels.find(param[0]);
    if (it != _channels.end())
    {
      std::cout << ":::::::::::::::::::: Chanel Found :::::::" << param[0]
                << std::endl;
      (param.size() > 1) ? _channels[param[0]].joinChannel(&client, param[1])
                         : _channels[param[0]].joinChannel(&client, "");
    }
    else
    {
      std::cout << ":::::::::::::::::::: Creating Channel :::::::" << param[0]
                << std::endl;
      createChannel(param[0]);
      it = _channels.find(param[0]);
      it->second.joinChannel(&client, "");
    }
    client.getMessage(
        Replies::RPL_WELCOME(client.getNickName(), client.getName(), param[0]));
  }
  catch (std::exception &e)
  {
    std::cout << "Error :::" << e.what() << std::endl;
    client.getMessage(e.what());
  }
}
```

## Handle Message on Client Side
```cpp
std::string Client::receiveMessage() {
  char buffer[1024];
  int bytesReceived = recv(_clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived == -1) {
    std::cerr << "Client Disconnected" << std::endl;
    return ("");
  } else if (bytesReceived == 0) {
    std::cerr << "Client disconnected" << std::endl;
    return "";
  }
  std::cout << "Message received " << buffer << std::endl;
  return (buffer);
}
```

## Delete Client from Server
```cpp
void Server::cleanClient(int fd)
{
	try
	{
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end();)
		{
			if (it->getClientSoket() == fd)
			{
				it = _clients.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error clean Client");
	}
}
```