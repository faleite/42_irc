#include "Server.hpp"

// MESSAGES

std::string  ERR_NEEDMOREPARAMS(const std::string &cmd)
{
  return ("461 * " + cmd + " :Not enough parameters");
}

std::string  ERR_UNKNOWNCOMMAND(const std::string &cmd)
{
  return ("421 * " + cmd + " :Unknown command");
}

std::string  ERR_ALREADYREGISTRED()
{
  return("464 * :Password incorrect");
}

std::string  ERR_NOTREGISTERED()
{
  return("451 * :You need to register before you can use that command"); // ":You have not registered"
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
      
      // std::map<std::string, void (Server::*)
      // (Client&, const std::string &, const std::vector<std::string>&)>::iterator it = commandMap.find(cmd);
      // if (it != commandMap.end()) 
      //   (this->*(it->second))(client, cmd, params);
      connectionRegister(client, cmd, params);
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
      sendWelcomeMessage(client);
      brodcastMessage(client.getName() + " Has Join to the server" );
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
  if (param.empty())
  {
    client.getMessage(ERR_NEEDMOREPARAMS(cmd));
    return ;
  }
  client.setNickName(param[0]);
}

void Server::user(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  // Params: `<username> <mode> <host (unused)> <realname>`
  if (param.size() < 4)
  {
    client.getMessage(ERR_NEEDMOREPARAMS(cmd));
    return ;
  }
  client.setName(param[0]); // <username>
}
int checkInvalidCommand(const std::string &cmd)
{
  // See if exist others commands already
  if (cmd == "JOIN" || cmd == "KICK" || cmd == "INVITE" 
  || cmd == "TOPIC" || cmd == "MODE" || cmd == "PRIVMSG")
    return (1);
  else if (cmd != "PASS" && cmd != "NICK" && cmd != "USER")
    return (2);
  return (0);
}

void Server::connectionRegister(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  int checkCmd = checkInvalidCommand(cmd);
  if (checkCmd)
  {
    if (checkCmd == 1)
      client.getMessage(ERR_NOTREGISTERED());
    if (checkCmd == 2)
      client.getMessage(ERR_UNKNOWNCOMMAND(cmd));
    return ;
  }

  if (param.empty())
  {
    client.getMessage(ERR_NEEDMOREPARAMS(cmd));
    return ;
  }

  if (cmd == "PASS")
  {
    if (!client.getAuthAttempted())
    {
      client.setAuthAttempted(true);
      if (param[0] == getPass())
        client.setAuthenticated(true);
      else
        client.setAuthenticated(false);
    }
  }
  else if (cmd == "NICK")
    client.setNickName(param[0]);
  else if (cmd == "USER")
  {
    if (param.size() < 4)
    {
      client.getMessage(ERR_NEEDMOREPARAMS(cmd));
      return ;
    }
    client.setName(param[0]);
  }
}

// Not use here
void Server::join(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;
  // <channel>
  std::string chName = param[0];
  std::cout << ":::::: CHANEL " << chName << std::endl;
  _channels[chName].joinChannel(&client, "");
}
