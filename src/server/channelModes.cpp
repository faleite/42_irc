#include "Server.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//______________________________________ TOPIC.

void Server::topic(Client &client, const std::string &cmd,
                   const std::vector<std::string> &param) {
  (void)cmd;
  if (param.size() < 2 || param[1].empty())
    return;

  if (!client.getIsOperator()) {
    client.getMessage("Only Operators can change the channel mode");
    return;
  }
  std::string mess;
  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i) {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();
  findChannel(param[0]) ? _channels[param[0]].setTopic(mess)
                        : client.getMessage("Channel not found");
}

//______________________________________ MODE OF THE CHANNEL.

void Server::mode(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {

  (void)cmd;

  if (param.size() < 2 || param[1].empty() || !client.getIsOperator())
    return;

  if (findChannel(param[0])) {
    _channels[param[0]].isOnChannel(&client)
        ? _channels[param[0]].mode(&client, param[1], param)
        : client.getMessage("You are not in the channel");
  } else
    client.getMessage("Channel not found");
}

//_____________________________________ LIMIT OF THE CHANNEL.

//_________________Password.