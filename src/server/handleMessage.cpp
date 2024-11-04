#include "Server.hpp"
#include <exception>
#include <ostream>
#include <sys/socket.h>

// #_____________________________________________________ UTILIIES.

void Server::sendWelcomeMessage(Client newClient) {
  std::ostringstream oss;
  const std::string blue = "\033[34m";
  const std::string reset = "\033[0m";

  oss << "\nHello, " << newClient.getName() << "!\n"
      << "Welcome to the **best IRC Server** around.\n"
      << blue << "\n"
      << "                       ▪  ▄▄▄   ▄▄·                     \n"
      << "                       ██ ▀▄ █·▐█ ▌▪                    \n"
      << "                       ▐█·▐▀▀▄ ██ ▄▄                    \n"
      << "                       ▐█▌▐█•█▌▐███▌                    \n"
      << "                       ▀▀▀.▀  ▀·▀▀▀                     \n"
      << "               .▄▄ · ▄▄▄ .▄▄▄   ▌ ▐·▄▄▄ .▄▄▄            \n"
      << "               ▐█ ▀. ▀▄.▀·▀▄ █·▪█·█▌▀▄.▀·▀▄ █·          \n"
      << "               ▄▀▀▀█▄▐▀▀▪▄▐▀▀▄ ▐█▐█•▐▀▀▪▄▐▀▀▄           \n"
      << "               ▐█▄▪▐█▐█▄▄▌▐█•█▌ ███ ▐█▄▄▌▐█•█▌          \n"
      << "                ▀▀▀▀  ▀▀▀ .▀  ▀. ▀   ▀▀▀ .▀  ▀          \n"
      << "\n"
      << reset << "We're thrilled to have you join our community!\n"
      << "\n"
      << "To get started, please set your name and nickname:\n"
      << "- Use `USER <YourName> <ip> <host> <surname>` to set your real "
         "data.\n"
      << "- Use `NICK <YourNickname>` to choose a nickname.\n"
      << "\n"
      << "Once you're all set, feel free to jump into our channels:\n"
      << "- **#general**: For general discussion and announcements.\n"
      << "- **#help**: For support and troubleshooting.\n"
      << "- **#random**: For casual conversation and fun.\n"
      << "\n"
      << "Enjoy your stay and reach out if you need any assistance. Let’s make "
         "this a great place together!\n"
      << "\n"
      << "Best,\n"
      << "The 42_IRC Team\n";

  std::string welcomeMessage = oss.str();
  newClient.getMessage(welcomeMessage);
}

std::string Server::getMessage(int fd) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  std::string message;
  int bytesRecv = recv(fd, buffer, 1024, 0);
  if (bytesRecv <= 0 || bytesRecv > 510) {
    std::cout << "Client on fd: " << fd << " Disconnected" << std::endl;
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
  if (cmd == "MSG") {
    // Call the function to handle message in channel.
  } else if (cmd == "PRIVMSG") {
    // call the function for handle private Message.
    privateMessage(client, ss);
  } else if (cmd == "LIST") {
    // list all the channels.
  } else if (cmd == "SEND") {
    // call the function to handle the file transfer
  } else {
    std::cerr << "COMAND NOT FOUND ::::: " << std::endl;
  }
}

void Server::handleMessage(int fd) {
  std::string message = this->getMessage(fd);
  for (size_t i = 0; i < _clients.size(); i++) {
    if (_clients[i].getSocket() == fd) {
      std::string mess;
      if (!parseHandler(_clients[i],
                        message)) // this func return 0 or 1 to error
        comunicationManager(&_clients[i], message);
    }
  }
  // brodcastMessage(message);
  std::cout << message;
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
