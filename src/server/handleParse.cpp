#include "Server.hpp"

std::vector<std::vector<std::string> > tokenization(const std::string &message) 
{
    std::vector<std::vector<std::string> > allTokens;
    std::istringstream iss(message);
    std::string line;

    while (getline(iss, line, '\n')) {
        std::vector<std::string> tokens;
        std::istringstream lineStream(line);
        std::string token;

        while (getline(lineStream, token, ' ')) {
            tokens.push_back(token);
        }
        allTokens.push_back(tokens);
    }
    return allTokens;
}

/**
 * Utils:
 * std::cout << "Name cannot be empty" << std::endl;
 * std::cout << "Nickname cannot be empty" << std::endl;
 */
int Server::parseHandler(Client &client, std::string &message)
{
    std::string msg = message;
    if (msg.empty() || msg == ".\r\n") 
        return 1;

    msg.erase(remove(msg.begin(), msg.end(), '\r'), msg.end());
    std::vector<std::vector<std::string> > allTokens = tokenization(msg);

    for (size_t i = 0; i < allTokens.size(); ++i) 
    {
      std::vector<std::string> tokens = allTokens[i];

      std::cout << "Line " << i << ":" << std::endl;
      for (size_t j = 0; j < tokens.size(); ++j) 
        std::cout << "  Token " << j << ": " << tokens[j] << std::endl;

      if (tokens.empty())
        continue;

      std::string command = tokens[0];
      std::vector<std::string> param(tokens.begin() + 1, tokens.end());

      if (command == "NICK")
      {
        if (client.getNickName().empty())
          client.getMessage(":" + param[0] + "!@localhost NICK :" + param[0]);
        else
          client.getMessage(":" + client.getNickName() + "!@localhost NICK :" + param[0]);
        client.setNickName(param[0]);
      }
      if (command == "USER")
        client.setName(param[0]);
      if (command == "PASS")
      {
        if (param[0] == getPass())
          client.setAuthenticated(true);
        else
          client.getMessage("ERROR :Invalid password");
      }
      if (command == "JOIN")
      {
        std::cout << ":::::: CHANEL " << param[0] << std::endl;
        _channels[param[0]].joinChannel(&client, "");
      }
    }
    // if (_clients[0].getAuthenticator())
    //   _clients[0].getMessage("Welcome " + _clients[0].getNickName() + "!" + _clients[0].getName() + "@localhost");
    return 0;
}
