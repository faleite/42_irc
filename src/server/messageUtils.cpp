#include "Server.hpp"
#include <exception>
#include <sstream>
#include <sys/socket.h>

void Server::privateMessage(Client *client, std::stringstream &commands) {
  std::string clientNick;
  commands >> clientNick;
  bool found;

  found = false;
  std::string mess;
  std::getline(commands, mess);
  for (std::vector<Client>::iterator it = _clients.begin();
       it != _clients.end(); ++it) {
    if (it->getNickName() == clientNick && client->getIsBot() == false) {
      it->getMessage(client->getNickName() +
                     " send you a private message: " + mess);
      found = true;
      break;
    } else if (it->getNickName() == clientNick && client->getIsBot() == true) {
      std::string cmdBot;
      commands >> cmdBot;
      if (cmdBot == "JOKE") {
        HTTPClient httpClient;
        std::string joke =
            httpClient.get("http://official-joke-api.appspot.com/jokes/random");
        it->getMessage("Here's a joke for you: " + joke);
        found = true;
      }
    }
  }
  if (found == false) {
    client->getMessage("Error:  ⚠️ " + clientNick + " not found.");
  }
}

void Server::fileTransfer(int const &clientFd, std::string const &paht) {
  try {
    File file(paht);
    char buffer[BUFFER_SIZE];
    while (int bytesRead = file.readChunkFile(buffer, sizeof(buffer))) {
      if (!file.sendFileTransfer(clientFd, buffer, bytesRead))
        break;

      std::string ackbytesRead;
      ackbytesRead = this->getMessage(clientFd);
      if (ackbytesRead.empty() || ackbytesRead == "AKC" != 0)
        break;
    }
    std::cout << "FILE TRANSFERED SUCCESSFULLY" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "Error during the file transfer " << e.what() << std::endl;
  }
}

// void Server::fileMessage(Client *client, std::string message) {
//   std::string cmd;
//   std::stringstream ss(message);

//   ss >> cmd;
//   if (cmd == "MSG") {
//     std::string channel;
//     ss >> channel;
//     std::map<std::string, Channel>::iterator it =
//         _channels.find(channel.substr(1));
//     if (it != _channels.end()) {
//       std::string mess;

//       std::getline(ss, mess);
//       std::cout << "siuuui" << client->getName() << std::endl;
//       it->second.brodcastMessage(mess);
//     }
//   } else if (cmd == "PRIVMSG") {
//     std::string clientNick;
//     ss >> clientNick;

//     std::string mess;
//     std::getline(ss, mess);
//     for (std::vector<Client>::iterator it = _clients.begin();
//          it != _clients.end(); ++it) {
//       if (it->getNickName() == clientNick) {
//         it->getMessage(client->getNickName() + "send you a message");
//         it->getMessage(mess);
//         break;
//       }
//     }
//   } else if (cmd == "LIST") {
//     // list all the channels.
//   } else if (cmd == "SEND") {
//     std::string filePath;
//     std::string clientNick;
//     ss >> clientNick;
//     ss >> filePath;
//     for (std::vector<Client>::iterator it = _clients.begin();
//          it != _clients.end(); ++it) {
//       if (it->getNickName() == clientNick) {
//         fileTransfer(it->getSocket(), "../file.txt");
//       }
//     }
//   }
// }

// void Server::botMessage(Client *client, std::string message) {
//   std::string cmd;
//   std::stringstream ss(message);

//   ss >> cmd;
//   if (cmd == "MSG") {
//     std::string channel;
//     ss >> channel;
//     std::map<std::string, Channel>::iterator it =
//         _channels.find(channel.substr(1));
//     if (it != _channels.end()) {
//       std::string mess;

//       std::getline(ss, mess);
//       std::cout << "siuuui" << client->getName() << std::endl;
//       it->second.brodcastMessage(mess);
//     }
//   } else if (cmd == "PRIVMSG") {
//     std::string clientNick;
//     ss >> clientNick;

//     std::string mess;
//     std::getline(ss, mess);
//     for (std::vector<Client>::iterator it = _clients.begin();
//          it != _clients.end(); ++it) {
//       if (it->getNickName() == clientNick) {
//         it->getMessage(client->getNickName() + "send you a message");
//         it->getMessage(mess);
//         break;
//       }
//     }
//   } else if (cmd == "LIST") {
//     // list all the channels.
//   } else if (cmd == "SEND") {
//     std::string filePath;
//     std::string clientNick;
//     ss >> clientNick;
//     ss >> filePath;
//     for (std::vector<Client>::iterator it = _clients.begin();
//          it != _clients.end(); ++it) {
//       if (it->getNickName() == clientNick) {
//         fileTransfer(it->getSocket(), "../file.txt");
//       }
//     }
//   }
// }
