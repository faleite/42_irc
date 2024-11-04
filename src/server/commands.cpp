#include "Server.hpp"

int Server::commands(Client &client, std::string &message) {
  std::string msg = message;
  if (msg.empty() || msg == ".\r\n")
    return 1;

  msg.erase(remove(msg.begin(), msg.end(), '\r'), msg.end());
  std::vector<std::vector<std::string> > allTokens = this->tokenization(msg);

  for (size_t i = 0; i < allTokens.size(); ++i) {
    std::vector<std::string> tokens = allTokens[i];
    if (tokens.empty())
      continue;

    std::string cmd = tokens[0];
    std::vector<std::string> params(tokens.begin() + 1, tokens.end());
    this->removeWhitespaceParams(params);

    for (size_t j = 0; j < cmd.length(); ++j)
      cmd[j] = toupper((unsigned char)cmd[j]);

    std::map<std::string,
             void (Server::*)(Client &, const std::string &,
                              const std::vector<std::string> &)>::iterator it =
        commandMap.find(cmd);
    if (it != commandMap.end())
      (this->*(it->second))(client, cmd, params);
  }
  return 0;
}

void Server::quit(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  std::ostringstream oss;
  for (size_t i = 0; i < param.size(); ++i) {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  std::string msg = oss.str();

  client.getMessage("ERROR :" + cmd + ": " + msg);
  std::cout << "Client disconnected with QUIT command :fd: "
            << client.getSocket() << std::endl;
  cleanClient(client.getSocket());
  close(client.getSocket());
}

void Server::pass(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  if (param.empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  client.getMessage(Replies::ERR_ALREADYREGISTRED());
}

void Server::user(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  if (param.size() < 4) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  client.getMessage(Replies::ERR_ALREADYREGISTRED());
}

void Server::nick(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  if (param.empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  for (size_t i = 0; i < _clients.size(); ++i) {
    if (param[0] == _clients[i].getNickName()) {
      client.getMessage(Replies::ERR_NICKNAMEINUSE(param[0]));
      return;
    }
  }
  client.getMessage(":" + client.getNickName() + "!" + client.getName() +
                    "@localhost " + cmd + " " + param[0]);
  client.setNickName(param[0]);
}

// Talk about this
void Server::join(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  (void)cmd;
  try {
    (param.size() > 1) ? _channels[param[0]].joinChannel(&client, param[1])
                       : _channels[param[0]].joinChannel(&client, "");
  } catch (std::exception &e) {
    std::cout << "Error :::" << e.what() << std::endl;
  }
}
