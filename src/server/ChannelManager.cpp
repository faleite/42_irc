#include "Server.hpp"

void Server::createChannel(std::string const &name) {
  _channels[name] = Channel(name);
}

bool Server::findChannel(std::string const &channelName) {
  std::map<std::string, Channel>::iterator it;
  it = _channels.find(channelName);
  return (it != _channels.end() ? true : false);
}

void Server::list(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  (void)cmd;
  (void)param;
  std::string response = "Active Channels:\n";
  response += "| Channel Name | Users Count | Topic/Description |\n";
  response += "|--------------------|-------------|--------------------------|"
              "\n";
  for (std::map<std::string, Channel>::iterator it = _channels.begin();
       it != _channels.end(); ++it) {
    std::string channelName = it->first;
    int userCount = it->second.getUsers();
    std::string topic = it->second.getTopic();

    std::stringstream row;
    row << "| " << std::left << std::setw(19) << channelName << "| "
        << std::left << std::setw(12) << userCount << "| " << std::left
        << std::setw(24) << topic << "|\n";
    response += row.str();
  }
  client.getMessage(response);
}

void Server::join(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  (void)cmd;
  try {

    std::map<std::string, Channel>::iterator it = _channels.find(param[0]);
    if (it != _channels.end()) {
      std::cout << ":::::::::::::::::::: Chanel Found :::::::" << param[0] << std::endl;
      (param.size() > 1) ? _channels[param[0]].joinChannel(&client, param[1])
                         : _channels[param[0]].joinChannel(&client, "");
    } else {
      std::cout << ":::::::::::::::::::: Creating Channel :::::::" << param[0] << std::endl;
      createChannel(param[0]);
      it = _channels.find(param[0]);
      it->second.joinChannel(&client, "");
    }
  } catch (std::exception &e) {
    std::cout << "Error :::" << e.what() << std::endl;
    client.getMessage(e.what());
  }
  list(client, cmd, param);
}
