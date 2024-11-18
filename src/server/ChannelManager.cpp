#include "Server.hpp"
#include <stdexcept>
#include <string>

//_______________________________________________CREATE CHANNEL

void Server::createChannel(std::string const &name) {
  _channels[name] = Channel(name);
}

bool Server::findChannel(std::string const &channelName) {
  std::map<std::string, Channel>::iterator it;
  it = _channels.find(channelName);
  return (it != _channels.end() ? true : false);
}

void Server::privmsg(Client &client, const std::string &cmd,
                     const std::vector<std::string> &param) {
  if (param.empty() || param.size() < 2) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }

  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i) {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  std::string mess = oss.str();

  if (param[0][0] == '#') {
    if ((this->_channels.find(param[0]) == this->_channels.end()) ||
        !this->_channels[param[0]].isOnChannel(client.getNickName())) {
      client.getMessage(
          Replies::ERR_NOSUCHCHANNEL(client.getNickName(), param[0]));
      return;
    }
    std::string send_msg =
        (":" + client.getNickName() + "!" + client.getName() + "@localhost " +
         cmd + " " + param[0] + mess);
    this->_channels[param[0]].broadcastMessage(send_msg, &client);
  }

  size_t i = 0;
  while (i < this->_clients.size()) {
    if (this->_clients[i]->getNickName() == param[0]) {
      std::string send_msg =
          (":" + client.getNickName() + "!" + client.getName() + "@localhost " +
           cmd + " " + param[0] + mess);
      this->_clients[i]->getMessage(send_msg);
      return;
    }

    i++;
    if (i == this->_clients.size() && param[0][0] != '#') {
      client.getMessage(Replies::ERR_NOSUCHNICK(client.getNickName()));
      return;
    }
  }
}

//______________________________________ MODE OF THE CHANNEL.

// Modes.

//___________Limit. l
// command : MODE <#CHANNEL> <FLAG> <NUMBER>
// EX :      MODE #GENERAL -l 10
// Param : 3 param[0] para[1] param[2]
// What we handle.

//___________ Topic t
// command : MODE <#channel> <flag>
// Ex : MODE #general -t // set the topic to private.

//___________ set privilege -o
// command : MODE <#channel> <flag> <userNickname>
// Ex : MODE #general -o user1

//___________Password.
// command : MODE <#CHANNEL> <FLAG> <pass>
// EX :      MODE #GENERAL +k pass
// Param : 3 param[0] para[1] param[2]
// What we handle.

void Server::mode(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {

  (void)cmd;
  std::cout << "hello :::::::::::: MOD COMMAND" << std::endl;

  if (param.size() < 2)
  {
    // client.getMessage(Replies::ERR_NEEDMOREPARAMS(param[0]));
    return ;
  }
  if (!findChannel(param[0])) {
    client.getMessage(Replies::ERR_NOSUCHCHANNEL(client.getNickName(), param[0]));
    return ;
  }
  if (!_channels[param[0]].isOnChannel(client.getNickName())) {
    client.getMessage(Replies::ERR_NOTONCHANNEL(param[0]));
    return ;
  }
  if (!client.getIsOperator()) {
    client.getMessage(Replies::ERR_CHANOPRIVSNEEDED(param[0]));
    return ;
  }
  _channels[param[0]].mode(&client, param[1], param);
}

//_____________________________________ LIMIT OF THE CHANNEL.

//_________________Password.

// void Server::mode(Client &client, const std::string &cmd,
//                   const std::vector<std::string> &param) {

//   (void)cmd;
//   std::cout << "hello :::::::::::: MOD COMMAND" << std::endl;

//   if (param.size() < 2 || param[1].empty())
//     return;
//   if (!client.getIsOperator())
//     client.getMessage(Replies::ERR_CHANOPRIVSNEEDED(param[0]));
//   if (findChannel(param[0])) {
//     _channels[param[0]].isOnChannel(client.getNickName())
//         ? _channels[param[0]].mode(&client, param[1], param)
//         : client.getMessage("You are not in the channel");
//   } else
//     client.getMessage("Channel not found");
// }