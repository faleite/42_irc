#include "Channel.hpp"
#include <vector>

Channel::Channel(std::string const &name) : _name(name) {
  std::cout << "The Channel < " << name << " > was created";
}

Channel::~Channel() { std::cout << "Channel Destroyed" << std::endl; }

// GETTERS
std::string const Channel::getName(void) { return (_name); }
std::string const Channel::getTopic(void) { return (_topic); }

// SETTER.
void Channel::setTopic(std::string const &topic) { this->_topic = topic; }

// ACTIONS.
void Channel::joinChannel(Client *newClient) {
  // Check if is the first client to make ir operator.
  std::vector<Client *>::iterator it;
  it = std::find(channelUsers.begin(), channelUsers.end(), newClient);
  if (channelUsers.size() == 0) {
    std::cout << "First client Ever, you are the boss." << std::endl;
    newClient->setOperator(true);
    channelUsers.push_back(newClient);
  } else if (it ==
             channelUsers
                 .end()) // can start in 2, as the bot can be create by default
  {
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
}