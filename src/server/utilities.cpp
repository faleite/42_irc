#include "../client/Client.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

bool checkAuthenticator(Client &client, std::string &command,
                        std::string &pass) {
  std::istringstream stream(command);
  std::string password;
  stream >> password;
  if (password == pass) {
    std::cout << "Corret Password " << std::endl;
    client.setAuthenticated(true);
    return true;
  } else {
    std::cout << "Wrong Password" << std::endl;
  }
  return false;
}
int parseHandler(Client &client, std::string &message) {

  /// Passe for moment.
  std::string passe = "pass";
  // 1. Check the authenticator.
  std::string cmd;
  std::istringstream stream(message);

  if (client.getAuthenticator() == true) {
    if (cmd == "USER" || cmd == "/USER")
    {
        std::string name;
        stream >> name;
        if (!name.empty())
            client.setName(name);
        else {
            std::cout << "Name can not be empty" << std::endl;
        }
    }
    else if (cmd == "NICK" || cmd == "/NICK")
    {
        std::string nickName;
        stream >> nickName;
        if (nickName.empty())
            client.setNickName(nickName);
    }
    else {
        std::cout << "Comand not recognised" << std::endl;
        
    }
  } else {
    if (cmd == "PASS" || cmd == "/PASS") {
      client.setAuthenticated(checkAuthenticator(client, cmd, passe));
    } else {
      std::cout << "Please set the password " << std::endl;
    }
  }
}


/// save function 

// void Server::comunicationManager(Client *client, std::string message) {
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