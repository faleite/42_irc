#include "../Server.hpp"

int Server::commands(Client &client, std::string &message)
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
      std::map<std::string, void (Server::*)(Client &, const std::string &,
      const std::vector<std::string> &)>::iterator it = commandMap.find(cmdName);
      if (it != commandMap.end())
      {
        (this->*(it->second))(client, cmdName, tokens);
        return (0);
      }
    }
  }
  return (1);
}

void Server::quit(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  std::ostringstream oss;
  for (size_t i = 0; i < param.size(); ++i)
  {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  std::string msg = oss.str();

  client.getMessage("ERROR :" + cmd + ": " + msg);
  std::cout << "Client disconnected with QUIT command :fd: "
            << client.getSocket() << std::endl;
  for (std::map<std::string, Channel>::iterator it = _channels.begin();
    it != _channels.end(); ++it) 
    it->second.leaveChannel(&client);
  cleanClient(client.getSocket());
  close(client.getSocket());
  delete &client;
}

void Server::pass(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  if (param.empty())
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  client.getMessage(Replies::ERR_ALREADYREGISTRED());
}

void Server::user(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  if (param.size() < 4)
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  client.getMessage(Replies::ERR_ALREADYREGISTRED());
}

void Server::nick(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  if (param.empty())
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  for (size_t i = 0; i < _clients.size(); ++i)
  {
    if (param[0] == _clients[i]->getNickName())
    {
      client.getMessage(Replies::ERR_NICKNAMEINUSE(param[0]));
      return;
    }
  }
  client.getMessage(":" + client.getNickName() + "!" + client.getName() +
                    "@localhost " + cmd + " " + param[0]);
  client.setNickName(param[0]);
}
