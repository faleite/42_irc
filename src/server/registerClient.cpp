#include "Server.hpp"

int Server::connectionRegistration(Client &client, std::string &message)
{
  client.getBuffer() += message;

  std::size_t pos;
  while ((pos = client.getBuffer().find("\n")) != std::string::npos)
  {
    std::string msg = client.getBuffer().substr(0, pos);
    client.setBuffer(client.getBuffer().substr(pos + 1));
    if (!msg.empty() && msg[0] == ':')
      msg = msg.substr(1);

    std::stringstream ss(msg);
    std::string cmd;
    std::string params;
    while (std::getline(ss, cmd))
    {
      if (!cmd.empty() && cmd[cmd.size() - 1] == '\r')
        cmd.erase(cmd.size() - 1);
      std::string cmdName = cmd.substr(0, cmd.find(" "));
      for (size_t i = 0; i < cmdName.length(); ++i)
        cmdName[i] = toupper((unsigned char)cmdName[i]);

      if (cmd.find(" ") != std::string::npos)
        params = cmd.substr(cmd.find(" ") + 1);

      std::stringstream streamTokens(params);
      std::string token;
      std::vector<std::string> tokens;
      while (std::getline(streamTokens, token, ' '))
      {
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        tokens.push_back(token);
      }
      this->registerNewUser(client, cmdName, tokens);
    }
  }
  if (!client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
  {
    std::cout << "Client has been disconnected :fd: " << client.getSocket() << std::endl;
    client.getMessage(Replies::ERR_PASSWDMISMATCH());
    this->cleanClient(client.getSocket());
    close(client.getSocket());
    return (1);
  }
  if (client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
  {
    client.setRegistered(true);
    this->sendWelcomeMessage(client);
  }
  return (0);
}

int checkInvalidCommand(const std::string &cmd)
{
  // See if exist others commands already
  if (cmd == JOIN || cmd == KICK || cmd == INVITE || cmd == LIST || cmd == MODE || cmd == PRIVMSG || cmd == SEND)
    return (1);
  else if (cmd != PASS && cmd != NICK && cmd != USER && cmd != QUIT)
    return (2);
  return (0);
}

void Server::registerNewUser(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  if (cmd == "CAP")
    return ;

  int checkCmd = checkInvalidCommand(cmd);
  
  if (checkCmd)
  {
    if (checkCmd == 1)
      client.getMessage(Replies::ERR_NOTREGISTERED());
    if (checkCmd == 2)
      client.getMessage(Replies::ERR_UNKNOWNCOMMAND(cmd));
    return;
  }
  if (cmd == QUIT)
    this->quit(client, cmd, param);
  if (param.empty())
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  if (cmd == PASS)
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
  else if (cmd == NICK)
  {
    for (size_t i = 0; i < _clients.size(); ++i)
    {
      if (param[0] == _clients[i]->getNickName())
      {
        client.getMessage(Replies::ERR_NICKNAMEINUSE(param[0]));
        return;
      }
    }
    client.setNickName(param[0]);
  }
  else if (cmd == USER)
  {
    if (param.size() < 4)
    {
      client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
      return;
    }
    client.setName(param[0]);
  }
}
