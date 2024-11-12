#include "Server.hpp"
#include <stdexcept>
#include <string>

//_______________________________________________CREATE CHANNEL

void Server::createChannel(std::string const &name)
{
  _channels[name] = Channel(name);
}

bool Server::findChannel(std::string const &channelName)
{
  std::map<std::string, Channel>::iterator it;
  it = _channels.find(channelName);
  return (it != _channels.end() ? true : false);
}

/* 
<< PRIVMSG #local :hello
>> :juan!pma@localhost PRIVMSG #local :hello
*/
void Server::privmsg(Client &client, const std::string &cmd,
                     const std::vector<std::string> &param) {
  if (param.empty() || param.size() < 2)
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return ;
  }

  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i) {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  std::string mess = oss.str();
  
  if (param[0][0] == '#') {
    if (this->_channels.find(param[0]) == this->_channels.end()) {
      client.getMessage(Replies::ERR_NOSUCHCHANNEL(client.getNickName(), param[0]));
      return ;
    }
    std::string send_msg = (":" + client.getNickName() + "!" + client.getName() 
                            + "@localhost " + cmd + " " + param[0] + mess);
    this->_channels[param[0]].broadcastMessage(send_msg, &client);
    return ;
  }

  size_t i = 0;
  while (i < this->_clients.size())
  {
    if (this->_clients[i]->getNickName() == param[0])
    {
      std::string send_msg = (":" + client.getNickName() + "!" + client.getName() 
                            + "@localhost " + cmd + " " + param[0] + mess);
      this->_clients[i]->getMessage(send_msg);
      return ;
    }
    
    i++;
    if (i == this->_clients.size())
    {
      client.getMessage(Replies::ERR_NOSUCHNICK(client.getNickName()));
      return ;
    }
  }
}

//________________________________SEND MESSAGE IN THE CHANNEL

void Server::msg(Client &client, const std::string &cmd,
                 const std::vector<std::string> &param)
{
  (void)cmd;
  std::string mess = client.getName() + " : ";
  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i)
  {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();
  if (findChannel(param[0]))
  {
    _channels[param[0]].isOnChannel(client.getNickName())
        ? _channels[param[0]].broadcastMessage(mess, &client)
        : client.getMessage("You are not part of this channel");
  }
  else
    client.getMessage("Channel not found");
}