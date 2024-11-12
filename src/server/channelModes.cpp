#include "Server.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


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
                  const std::vector<std::string> &param)
{

  (void)cmd;
  std::cout << "hello :::::::::::: MOD COMMAND" << std::endl;

  if (param.size() < 2 || param[1].empty() || !client.getIsOperator())
    return;

  if (findChannel(param[0]))
  {
    _channels[param[0]].isOnChannel(client.getNickName())
        ? _channels[param[0]].mode(&client, param[1], param)
        : client.getMessage("You are not in the channel");
  }
  else
    client.getMessage("Channel not found");
}

//_____________________________________ LIMIT OF THE CHANNEL.

//_________________Password.