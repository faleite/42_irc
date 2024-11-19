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
  if (param.empty()) {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return;
  }

  if (this->_channels.find(param[0]) == this->_channels.end()) {
    client.getMessage(Replies::ERR_NOSUCHCHANNEL(
                      client.getNickName(), param[0]));
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

  std::vector<Client *> &clients = this->_channels[param[0]].getClients();
  for (std::vector<Client *>::iterator it = clients.begin(); 
      it != clients.end();) {
    if (*it == &client) 
      it = clients.erase(it);
    else 
      ++it;
  }
  this->_channels[param[0]].updateListUsers(&client);
  client.getMessage(Replies::PART_USER(cmd, client.getNickName(), 
                    client.getName(), param[0], mess));
}