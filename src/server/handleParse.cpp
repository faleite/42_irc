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
 * std::cout << "Line " << i << ":" << std::endl;
 * for (size_t j = 0; j < tokens.size(); ++j) 
 * std::cout << "  Token " << j << ": " << tokens[j] << std::endl;
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
      if (tokens.empty())
        continue;

      std::string cmd = tokens[0];
      std::vector<std::string> params(tokens.begin() + 1, tokens.end());

      std::map<std::string, void (Server::*)
      (Client&, const std::vector<std::string>&)>::iterator it = commandMap.find(cmd);
      if (it != commandMap.end()) 
        (this->*(it->second))(client, params);
      else
        client.getMessage("Unknown command: " + cmd);
    }
    // if (_clients[0].getAuthenticator())
    //   _clients[0].getMessage("Welcome " + _clients[0].getNickName() + "!" + _clients[0].getName() + "@localhost");
    return 0;
}

void Server::nick(Client &client, const std::vector<std::string> &param)
{
  // <NIck name>
  std::string nick = param[0];
  if (client.getNickName().empty())
    client.getMessage(":" + nick + "!@localhost NICK :" + nick);
  else
    client.getMessage(":" + client.getNickName() + "!@localhost NICK :" + nick);
  client.setNickName(nick);
}

void Server::user(Client &client, const std::vector<std::string> &param)
{
  // <userName> < host> <port> < surname>
  std::string user = param[0];
  client.setName(user);
}

void Server::pass(Client &client, const std::vector<std::string> &param)
{
  // <pass>
  std::string pass = param[0];
  if (pass == getPass())
    client.setAuthenticated(true);
  else
    client.getMessage("ERROR :Invalid password");
}

void Server::join(Client &client, const std::vector<std::string> &param)
{
  // <channel>
  std::string chName = param[0];
  std::cout << ":::::: CHANEL " << chName << std::endl;
  _channels[chName].joinChannel(&client, "");
}
