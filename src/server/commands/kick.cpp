#include "../Server.hpp"

// KICK <channel> <nickname>
//______________________________________ KICK.
// command : TOPIC <#CHANNEL> <nickname>.
// EX :      KICK #GENERAL user1
// EX :      KICK #channelname usernickname Reason
// Param : 3 param[0] para[1] param[2]
// What we handle.

// Remove the client from the channel

void Channel::leaveChannel(Client *client, const std::string &mess) {
  (void)mess;
  std::vector<Client *>::iterator it =
      std::find(channelUsers.begin(), channelUsers.end(), client);
  if (it != channelUsers.end()) {
    channelUsers.erase(it);
    // broadcastMessage(Replies::KICK_USER(*client, _name, (*it)->getNickName(),
    // mess), client);
  }

  // treat when channel gets empty.
  // if (channelUsers.empty())
  //   _active = false;
}

void Server::kick(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  (void)cmd;

  if (param.size() < 2 || param[1].empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }
  if (!client.getIsOperator()) {
    client.getMessage(Replies::ERR_CHANOPRIVSNEEDED(param[0]));
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
  if (findChannel(param[0]) &&
      _channels[param[0]].isOnChannel(client.getNickName())) {
    for (std::vector<Client *>::iterator it = _clients.begin();
         it != _clients.end(); ++it) {
      if (((*it)->getNickName()) == param[1]) {
        _channels[param[0]].leaveChannel((*it), mess);

        // _____________________ Kick Message for the target.
        std::string kickMessage = ":jf.irc KICK " + param[0] + " " +
                                  (*it)->getNickName() +
                                  " :You have been kicked from the channel\r\n";
        (*it)->getMessage(kickMessage);
        // _____________________ Kick Message for the Channel.
        std::string notifyMessage =
            ":" + client.getNickName() + "!" + client.getName() + "@" +
            client.getIp() + " KICK " + param[0] + " " + (*it)->getNickName() +
            ": has been kicked from the channel\r\n";
        _channels[param[0]].broadcastMessage(notifyMessage, &client);

        // _____________________ Kick Message for the admin that kick the user.
        std::string adminMessage = "::jf.irc KICK " + param[0] + " " +
                                   (*it)->getNickName() +
                                   " :Client has been kicked\r\n";
        client.getMessage(adminMessage);

        //     Replies::KICK_USER(client, param[1], (*it)->getName(), mess),
        //     (*it));
        // (*it)->getMessage(
        //     Replies::KICK_USER(client, param[1], (*it)->getName(), mess));
        return;
      }
      // client.getMessage(Replies::ERR_USERNOTINCHANNEL(client.getNickName(),
      // param[0], param[1]));
    }
  } else {
    client.getMessage(
        Replies::ERR_NOSUCHCHANNEL(client.getNickName(), param[0]));
  }
  return;
}