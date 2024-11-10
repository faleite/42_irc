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

void Server::list(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  (void)cmd;
  (void)param;
  std::string response = "Active Channels:\n";
  response += "| Channel Name | Users Count | Topic/Description |\n";
  response += "|--------------------|-------------|--------------------------|"
              "\n";
  for (std::map<std::string, Channel>::iterator it = _channels.begin();
       it != _channels.end(); ++it)
  {
    std::string channelName = it->first;
    int userCount = it->second.getUsers();
    std::string topic = it->second.getRestrictedTopic() ? "Restricted Topic"
                                                      : it->second.getTopic();

    std::stringstream row;
    row << "| " << std::left << std::setw(19) << channelName << "| "
        << std::left << std::setw(12) << userCount << "| " << std::left
        << std::setw(24) << topic << "|\n";
    response += row.str();
  }
  client.getMessage(response);
}

//_____________________________JOIN TO A CHANNEL________________________

std::vector<std::string> channelsTokenization(const std::string &param) 
{
    std::vector<std::string> channels;
    std::istringstream iss(param);
    std::string channel;

    while (getline(iss, channel, ',')) {
        channels.push_back(channel);
    }       
    return channels;
}

void Server::join(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  if (param.empty())
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return ;
  }

  if (!param[0].compare("0"))
  {
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
      client.getMessage(Replies::LEAVE_CHANNEL(client.getNickName(), client.getName(), it->first));
      it->second.leaveChannel(&client);
    }
    return ;
  }

  std::vector<std::string> channels = channelsTokenization(param[0]);
  for (size_t i = 0; i < channels.size(); ++i)
  {
    if (channels[i][0] != '#')
    {
      client.getMessage(Replies::ERR_NOSUCHCHANNEL(client.getNickName(), channels[i]));
      continue ;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(channels[i]);
    if (it != _channels.end())
    {
      std::cout << ":::::::: Channel Found :::::::: " << channels[i] << std::endl;
      (param.size() > 1) ? _channels[channels[i]].joinChannel(&client, param[1])
                        : _channels[channels[i]].joinChannel(&client, "");
    }
    else
    {
      std::cout << "::::::: Creating Channel ::::::: " << channels[i] << std::endl;
      createChannel(channels[i]);
      it = _channels.find(channels[i]);
      it->second.joinChannel(&client, "");
    }
  }
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