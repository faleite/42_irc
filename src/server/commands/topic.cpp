#include "../Server.hpp"

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
    if (findChannel(param[0]) &&
        _channels[param[0]].isOnChannel(client.getNickName()))
    {
      _channels[param[0]].getRestrictedTopic()
          ? client.getMessage("The topic of this channel is restricted")
          : client.getMessage("the topic of the channel is :" +
                              _channels[param[0]].getTopic());
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