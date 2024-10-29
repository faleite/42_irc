#include "Server.hpp"

/**
 * Utils:
 * std::cout << "Name cannot be empty" << std::endl;
 * std::cout << "Nickname cannot be empty" << std::endl;
 */
int Server::parseHandler(Client &client, std::string &message)
{
  std::string cmd;
  std::istringstream stream(message);
  stream >> cmd;

  if (cmd == "JOIN")
  {
    std::string channel;
    stream >> channel;
    std::cout << ":::::: CHANEL " << channel << std::endl;
    channelManager(&client, channel);
  }
  if (cmd == "USER")
  {
    std::string name;
    stream >> name;
    if (!name.empty())
    {
      client.setName(name);
      std::cout << "NAME :: " << client.getName() << std::endl;
    }
  }
  if (cmd == "NICK")
  {
    std::string nickName;
    stream >> nickName;
    if (!nickName.empty())
    {
      if (client.getNickName().empty())
        client.getMessage(":" + nickName + "!@localhost NICK :" + nickName);
      else
        client.getMessage(":" + client.getNickName() + "!@localhost NICK :" + nickName);
      client.setNickName(nickName);
    }
  }
  if (cmd == "PASS")
  {
    std::string password;
    stream >> password;
    if (!password.empty())
      client.setAuthenticated(checkAuthenticator(client, password));
  }
  return 0;
}