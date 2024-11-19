#include "../Server.hpp"

void Channel::leaveChannel(Client *client) {
  std::vector<Client *>::iterator it =
      std::find(channelUsers.begin(), channelUsers.end(), client);
  if (it != channelUsers.end()) {
    channelUsers.erase(it);
  }
}


void Server::kick(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  if (param.size() < 3) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  if (!client.getIsOperator()) {
    client.getMessage(Replies::ERR_CHANOPRIVSNEEDED(param[0]));
    return;
  }
  if (!findChannel(param[0])) {
    client.getMessage(Replies::ERR_NOSUCHCHANNEL(
                      client.getNickName(), 
                      param[0]));
    return;
  }

  std::string mess;
  std::ostringstream oss;
  for (size_t i = 2; i < param.size(); ++i) {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();

  if (!_channels[param[0]].isOnChannel(param[1])) {
    client.getMessage(Replies::ERR_USERNOTINCHANNEL(
                      client.getNickName(), 
                      param[0], param[1]));
    return;
  } else {
    for (size_t i = 0; i < this->_clients.size(); i++) {
      if (this->_clients[i]->getNickName() == param[1]) {
        this->_clients[i]->getMessage(Replies::KICK_USER(
                                      cmd, client.getNickName(), 
                                      client.getName(), param[0], 
                                      param[1], mess));
        client.getMessage(Replies::KICK_USER(
                          cmd, client.getNickName(), 
                          client.getName(), param[0], 
                          param[1], mess));
        _channels[param[0]].broadcastMessage(Replies::KICK_USER(
                                              cmd, client.getNickName(), 
                                              client.getName(), param[0], 
                                              param[1], mess), &client);
        _channels[param[0]].leaveChannel(this->_clients[i]);
        if (this->_clients[i]->getIsOperator()){
          this->_clients[i]->setOperator(false);
        }
        this->_channels[param[0]].updateListUsers(this->_clients[i]);
        return; 
      }
    }
  }
}