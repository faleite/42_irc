#include "../Server.hpp"

// PART

// PART Command Format

//     Message Format: :<nickname>!<username>@<hostname> PART <channel>
//     [:<reason>]

//         <nickname>: The nickname of the user leaving the channel.

//         <username>: The username of the user.

//         <hostname>: The hostname of the user.

//         <channel>: The name of the channel being left.

//         <reason> (optional): An optional message explaining why the user is
//         leaving.

// Example:

//     Example Message: :Alice!alice@localhost PART #general :Goodbye!

//         Translation: Alice is leaving the channel #general with the message
//         "Goodbye!".

void Server::part(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  // std::vector<std::string> channel_list;

  if (param.empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
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
  // _channels[param[0]].broadcastMessage(Replies::PART_USER(cmd,
  //                                                         client.getNickName(),
  //                                                         client.getName(),
  //                                                         _channels[param[0]].getName(),
  //                                                         mess),
  //  &client);
  _channels[param[0]].leaveChannel(&client, mess);
  std::string notifyMessage = ":" + client.getNickName() + "!" +
                              client.getName() + "@" + client.getIp() +
                              " KICK " + param[0] + " " + client.getNickName() +
                              ": " + mess + "\r\n";
  _channels[param[0]].broadcastMessage(mess, &client);
  std::string adminMessage = ":jf.irc KICK " + param[0] + " " +
                             client.getNickName() + " :Client has leave\r\n";
  client.getMessage(adminMessage);
  return;

  // for (size_t i = 0; i < param.size(); ++i)
  // {
  //   if (param[i][0] != '#')
  //   {
  //     if (i != 0)
  //       oss << " ";
  //     oss << param[i];
  //   }
  //   else
  //   {
  //     if (findChannel(param[i]) &&
  //     _channels[param[i]].isOnChannel(client.getNickName()))
  //     {
  //       channel_list.push_back(param[i]);
  //     }
  //   }
  // }

  // for (std::vector<std::string>::iterator it = channel_list.begin();
  //      it != channel_list.end(); ++it)
  // {
  // (client).getMessage(
  //     Replies::KICK_USER(client, *it, client.getName(), mess));
  // }
}