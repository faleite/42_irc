#include "../Server.hpp"

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
                  const std::vector<std::string> &param) {
  if (param.empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }

  if (!param[0].compare("0")) {
    for (std::map<std::string, Channel>::iterator it = _channels.begin();
         it != _channels.end(); ++it) {
      client.getMessage(Replies::LEAVE_CHANNEL(client.getNickName(),
                                               client.getName(), it->first));
      it->second.leaveChannel(&client);
    }
    return;
  }

  std::vector<std::string> channels = channelsTokenization(param[0]);
  for (size_t i = 0; i < channels.size(); ++i) {
    if (channels[i][0] != '#') {
      client.getMessage(
          Replies::ERR_NOSUCHCHANNEL(client.getNickName(), channels[i]));
      continue;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(channels[i]);
    if (it != _channels.end()) {
      std::cout << ":::::::: Channel Found :::::::: " << channels[i]
                << std::endl;
      (param.size() > 1) ? _channels[channels[i]].joinChannel(&client, param[1])
                         : _channels[channels[i]].joinChannel(&client, "");
    } else {
      std::cout << "::::::: Creating Channel ::::::: " << channels[i]
                << std::endl;
      createChannel(channels[i]);
      it = _channels.find(channels[i]);
      it->second.joinChannel(&client, "");
    }
  }
}