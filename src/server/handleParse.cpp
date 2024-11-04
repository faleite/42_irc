#include "Server.hpp"

// MESSAGES

std::string  ERR_NEEDMOREPARAMS(const std::string &cmd)
{
  std::string msg = "461 " + cmd + " :Not enough parameters";
  return (msg);
}

std::string  ERR_ALREADYREGISTRED()
{
  std::string msg = "464 * :Password incorrect";
  return (msg);
}

// 

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

bool removeWhitespaceParams(std::vector<std::string> &params) 
{
  bool vectorEmpty = true;
  for (size_t j = 0; j < params.size(); ++j) 
  {
    if (!params[j].empty() && params[j].find_first_not_of(" \t\n\v\f\r") != std::string::npos) 
    {
        vectorEmpty = false;
        break;
    }
  }
  if (vectorEmpty)
    params.clear();
  return vectorEmpty;
}

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
      removeWhitespaceParams(params);

      for (size_t j = 0; j < cmd.length(); ++j) 
        cmd[j] = toupper((unsigned char)cmd[j]);
      
      std::map<std::string, void (Server::*)
      (Client&, const std::string &, const std::vector<std::string>&)>::iterator it = commandMap.find(cmd);
      if (it != commandMap.end()) 
        (this->*(it->second))(client, cmd, params);
    }
    if (!client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
    {
      std::cout << "Client on fd: " << client.getSocket() << " Disconnected" << std::endl;
      client.getMessage(ERR_ALREADYREGISTRED());
      cleanClient(client.getSocket());
      close(client.getSocket());
      return (1);
    }
    if (client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
    {
      if (client.getIsWelcome() == false)
      {
        sendWelcomeMessage(client);
        brodcastMessage(client.getName() + " Has Join to the server" );
        client.setIsWelcome(true);
      }
    }
    return 0;
}

void Server::pass(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  if (param.empty())
  {
    client.getMessage(ERR_NEEDMOREPARAMS(cmd));
    return ;
  }
  if (!client.getAuthAttempted())
  {
    client.setAuthAttempted(true);
    if (param[0] == getPass())
      client.setAuthenticated(true);
    else
      client.setAuthenticated(false);
  }
}

void Server::nick(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;
  client.setNickName(param[0]);
}

void Server::user(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;
  // <userName> < host> <port> < surname>
  std::string user = param[0];
  client.setName(user);
}

// void Server::nick(Client &client, const std::vector<std::string> &param)
// {
//   // <NIck name>
//   std::string nick = param[0];
//   if (client.getNickName().empty())
//     client.getMessage(":" + nick + "!@localhost NICK :" + nick);
//   else
//     client.getMessage(":" + client.getNickName() + "!@localhost NICK :" + nick);
//   client.setNickName(nick);
// }

// void Server::user(Client &client, const std::vector<std::string> &param)
// {
//   // <userName> < host> <port> < surname>
//   std::string user = param[0];
//   client.setName(user);
// }

// void Server::pass(Client &client, const std::vector<std::string> &param)
// {
//   // <pass>
//   std::string pass = param[0];
//   if (pass == getPass())
//     client.setAuthenticated(true);
//   else
//     client.getMessage("ERROR :Invalid password");
// }

void Server::join(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;
  // <channel>
  std::string chName = param[0];
  std::cout << ":::::: CHANEL " << chName << std::endl;
  _channels[chName].joinChannel(&client, "");
}
