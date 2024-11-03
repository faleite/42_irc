#include "Server.hpp"
#include <exception>
#include <sys/socket.h>

// #_____________________________________________________ UTILIIES.

void Server::sendWelcomeMessage(Client newClient) {
  std::ostringstream oss;
  const std::string blue = "\033[34m";
  const std::string reset = "\033[0m";

  oss <<"\nHello, " << newClient.getNickName() << "!\n"
      << "Welcome to the **best IRC Server** around.\n"
      << blue
      <<"\n"
      <<"               ▐▄▄▄·▄▄▄   ▪  ▄▄▄   ▄▄·             \n"
      <<"                ·██▐▄▄·   ██ ▀▄ █·▐█ ▌▪            \n"
      <<"              ▪▄ ████▪    ▐█·▐▀▀▄ ██ ▄▄            \n"
      <<"              ▐▌▐█▌██▌.   ▐█▌▐█•█▌▐███▌            \n"
      <<"            ▀  ▀▀▀•▀▀▀  ▀ ▀▀▀.▀  ▀·▀▀▀  ▀          \n"
      <<"           .▄▄ · ▄▄▄ .▄▄▄   ▌ ▐·▄▄▄ .▄▄▄           \n"
      <<"           ▐█ ▀. ▀▄.▀·▀▄ █·▪█·█▌▀▄.▀·▀▄ █·         \n"
      <<"           ▄▀▀▀█▄▐▀▀▪▄▐▀▀▄ ▐█▐█•▐▀▀▪▄▐▀▀▄          \n"
      <<"           ▐█▄▪▐█▐█▄▄▌▐█•█▌ ███ ▐█▄▄▌▐█•█▌         \n"
      <<"            ▀▀▀▀  ▀▀▀ .▀  ▀. ▀   ▀▀▀ .▀  ▀         \n"
      <<"\n"
      << reset
      << "We're thrilled to have you join our community!\n"
      << "\n"
      << "Feel free to jump into our channels:\n"
      << "- **#general**: For general discussion and announcements.\n"
      << "- **#help**: For support and troubleshooting.\n"
      << "- **#random**: For casual conversation and fun.\n"
      << "\n"
      << "Enjoy your stay and reach out if you need any assistance.\n"
      << "Let’s make this a great place together!\n"
      << "\n"
      << "Best,\n"
      << "The JF.IRC Team, at 42 School Lisbon\n";

  std::string welcomeMessage = oss.str();
  newClient.getMessage(welcomeMessage);
}

std::string Server::getMessage(int fd) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  std::string message;
  int bytesRecv = recv(fd, buffer, 1024, 0);
  if (bytesRecv <= 0 || bytesRecv > 510) {
    std::cout << "Client has been disconnected :fd: " << fd << std::endl;
    cleanClient(fd);
    close(fd);
  } else
      message = std::string(buffer, bytesRecv);
  return (message);
}

void Server::brodcastMessage(std::string const &message) {

  for (std::vector<Client>::iterator it = _clients.begin();
       it != _clients.end(); it++) {
    it->getMessage(message);
  }
}

void Server::comunicationManager(Client *client, std::string message) {
  std::string cmd;
  std::stringstream ss(message);

  ss >> cmd;
  if (cmd == MSG) {
    std::string channel;
    ss >> channel;
    std::map<std::string, Channel>::iterator it =
        _channels.find(channel.substr(1));
    if (it != _channels.end()) {
      std::string mess;

      std::getline(ss, mess);
      std::cout << "siuuui" << client->getName() << std::endl;
      it->second.brodcastMessage(mess);
    }
  } else if (cmd == PRIVMSG) {
    std::string clientNick;
    ss >> clientNick;

    std::string mess;
    std::getline(ss, mess);
    for (std::vector<Client>::iterator it = _clients.begin();
         it != _clients.end(); ++it) {
      if (it->getNickName() == clientNick) {
        it->getMessage(client->getNickName() + "send you a message");
        it->getMessage(mess);
        break;
      }
    }
  } else if (cmd == LIST) {
    // list all the channels.
  } else if (cmd == SEND) {
    std::string filePath;
    std::string clientNick;
    ss >> clientNick;
    ss >> filePath;
    for (std::vector<Client>::iterator it = _clients.begin();
         it != _clients.end(); ++it) {
      if (it->getNickName() == clientNick) {
        fileTransfer(it->getSocket(), "../file.txt");
      }
    }
  }
}

void Server::handleMessage(int fd) {
  std::string message = this->getMessage(fd);
  for (size_t i = 0; i < _clients.size(); i++) {
    if (_clients[i].getSocket() == fd) {
      if (!_clients[i].getRegistered()) {
        connectionRegistration(_clients[i], message);
      } else {
        commands(_clients[i], message);
        comunicationManager(&_clients[i], message);
      }
    }
  }
  // brodcastMessage(message);
  // std::cout << message;
}

void Server::channelManager(Client *client, std::string &channelName) {
  std::string channNa;

  channNa = channelName.substr(1);
  std::map<std::string, Channel>::iterator it = _channels.find(channNa);
  std::cout << ":::::::: >>>>>> " << it->first << std::endl;
  if (it != _channels.end()) {
    std::cout << "Channel found " << std::endl;
    std::cout << client->getName() << " will join to this channel" << std::endl;
    if (it->second.getVerification() == false)
      it->second.joinChannel(client, "");
  } else {
    std::cout << "Channel created" << std::endl;
    createChannel(channNa);
    it = _channels.find(channNa);
    it->second.joinChannel(client, "");
  }
}

void Server::fileTransfer(int const &clientFd, std::string const &paht) {
  try {
    File file(paht);
    char buffer[BUFFER_SIZE];
    while (int bytesRead = file.readChunkFile(buffer, sizeof(buffer))) {
      if (!file.sendFileTransfer(clientFd, buffer, bytesRead))
        break;

      char ack[3]; // acknowledgement.
      int ackbytesRead = recv(clientFd, ack, sizeof(ack), 0);
      if (ackbytesRead <= 0 || std::strncmp(ack, "AKC", 3) != 0)
        break;
    }
  } catch (const std::exception &e) {
    std::cout << "Error during the file transfer " << e.what() << std::endl;
  }
}
