#include "Channel.hpp"
#include <vector>

Channel::Channel(std::string const &name)
    : active(true), _name(name), _topic("") {
  std::cout << "The Channel < " << name << " > was created" << std::endl;
}

Channel::~Channel() { std::cout << "Channel Destroyed" << std::endl; }

// GETTERS
std::string const &Channel::getName(void) const { return (_name); }
std::string const &Channel::getTopic(void) const { return (_topic); }

// SETTER.
void Channel::setTopic(std::string const &topic) { this->_topic = topic; }

// ACTIONS.
void Channel::joinChannel(Client *newClient) {
  // Check if is the first client to make ir operator.
  std::vector<Client *>::iterator it;
  it = std::find(channelUsers.begin(), channelUsers.end(), newClient);
  if (it ==
      channelUsers.end()) // can start in 2, as the bot can be create by default
  {
    if (channelUsers.empty()) {
      std::cout << "First client Ever, you are the boss." << std::endl;
      newClient->setOperator(true);
    }
    channelUsers.push_back(newClient);
    std::cout << " join the Channel " << newClient->getName() << std::endl;
  } else {
    std::cout << "The " << newClient->getName() << " Is already in the Channel"
              << std::endl;
  }
}
void Channel::leaveChannel(Client *client) {
  std::vector<Client *>::iterator it;
  it = std::find(channelUsers.begin(), channelUsers.end(), client);
  if (it != channelUsers.end()) {
    channelUsers.erase(it);
    std::cout << client->getName() << " Has left the Channel" << std::endl;
  }
  if (channelUsers.empty())
    active = false;
}

void Channel::brodcastMessage(std::string &message) {
  for (std::vector<Client *>::iterator it = channelUsers.begin();
       it != channelUsers.end(); ++it) {
    (*it)->sendMessage(message);
  }
}