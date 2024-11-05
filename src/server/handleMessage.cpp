#include "Server.hpp"
#include <exception>
#include <ostream>
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
      << reset
      << "\nWe're thrilled to have you join our community!\n"
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

void Server::handleMessage(int fd) {
  std::string message = this->getMessage(fd);
  for (size_t i = 0; i < _clients.size(); i++) {
    if (_clients[i].getSocket() == fd) {
      if (!_clients[i].getRegistered()) {
        connectionRegistration(_clients[i], message);
      } else {
        commands(_clients[i], message);
      }
    }
  }
}