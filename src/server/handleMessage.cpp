#include "Server.hpp"

// #_____________________________________________________ UTILIIES.

void Server::sendWelcomeMessage(Client newClient)
{
  std::ostringstream oss;
  oss << "////////////////////////////////////////////////////////////////////"
         "\n"
      << "//                                                                "
         "//\n"
      << "//                              WELCOME                           "
         "//\n"
      << "//                                TO                              "
         "//\n"
      << "//                            /// SERVER ///                      "
         "//\n"
      << "//                                                                "
         "//\n"
      << "////////////////////////////////////////////////////////////////////"
         "\n"
      << "\n"
      << "Hello, " << newClient.getName() << "!\n"
      << "\n"
      << "Welcome to the **best IRC Server** around. We're thrilled to have "
         "you join our community!\n"
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
  newClient.sendMessage(welcomeMessage);
}

std::string Server::getMessage(int fd)
{
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  std::string message;
  int bytesRecv = recv(fd, buffer, 1024, 0);
  if (bytesRecv <= 0 || bytesRecv > 510)
  {
    std::cout << "Client on fd: " << fd << " Disconnected" << std::endl;
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
      cleanClient(fd);
      close(fd);
    }
  }
  else
    message = std::string(buffer, bytesRecv);
  return (message);
}

void Server::handleMessage(int fd)
{
  std::string message = this->getMessage(fd);
  // // size_t i = 0;
  // // while (!_clients.empty() && i <_clients.size())
  // // {
        parseHandler(_clients[0], message);
  // // std::cout << "PORT :: " << _clients[0].getPort() << std::endl;
  // // i++;
  // // }
  std::cout << message;
}