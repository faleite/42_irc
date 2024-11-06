#include "Server.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//______________________________________ TOPIC.
// command : TOPIC <#CHANNEL> will list the topic of te channel.
// EX :      TOPIC #GENERAL
// TOPIC #chhannel <new topic> change the topic of the channel.
// Param : 3 param[0] para[1] param[2]
// What we handle.

void Server::topic(Client &client, const std::string &cmd,
                   const std::vector<std::string> &param)
{
  (void)cmd;
  if (param.size() == 1)
  {
    if (findChannel(param[0]) && _channels[param[0]].isOnChannel(&client))
    {
      _channels[param[0]].getRestrictedTopic() ? client.getMessage("The topic of this channel is restricted")
                                               : client.getMessage("the topic of the channel is :" + _channels[param[0]].getTopic());
    }
    return;
  }
  else if (param.size() < 2 || param[1].empty())
    return;
  if (!client.getIsOperator())
  {
    client.getMessage("Only Operators can change the channel mode");
    return;
  }
  std::string mess;
  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i)
  {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();
  findChannel(param[0]) ? _channels[param[0]].setTopic(mess)
                        : client.getMessage("Channel not found");
}
void Server::kick(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;
  
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

//___________Invite to a channel
// Command : IVITE <#channel> <user>
// invite username #channelname

//___________Password.
// command : MODE <#CHANNEL> <FLAG> <pass>
// EX :      MODE #GENERAL +k pass
// Param : 3 param[0] para[1] param[2]
// What we handle.

void Server::invite(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  (void)cmd;

  if (param.size() < 2 || param[1].empty() || !client.getIsOperator())
    return;
  if (findChannel(param[0]))
  {
    for (std::vector<Client>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
      if (iter->getNickName() == param[1])
      {

        _channels[param[0]].invite(&(*iter));
        return;
      }
    }
    client.getMessage("User Not Found");
  }
}

void Server::mode(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{

  (void)cmd;

  if (param.size() < 2 || param[1].empty() || !client.getIsOperator())
    return;

  if (findChannel(param[0]))
  {
    _channels[param[0]].isOnChannel(&client)
        ? _channels[param[0]].mode(&client, param[1], param)
        : client.getMessage("You are not in the channel");
  }
  else
    client.getMessage("Channel not found");
}

//_____________________________________ LIMIT OF THE CHANNEL.

//_________________Password.