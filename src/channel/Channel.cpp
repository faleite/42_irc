#include "Channel.hpp"
#include <vector>

Channel::Channel(std::string const &name)
    : needInvitation(false), needVerification(false), restricTopic(false),
      active(true), _name(name), _topic("") {
  std::cout << "The Channel < " << name << " > was created" << std::endl;
}

bool Channel::searchClient(Client *client, int use) {
  std::vector<Client *>::iterator iter;
  if (use == 1) {
    iter = std::find(channelUsers.begin(), channelUsers.end(), client);
    if (iter == channelUsers.end())
      return true;
    else
      return false;
  } else {
    iter = std::find(invitedList.begin(), invitedList.end(), client);
    if (iter == invitedList.end())
      return true;
    else
      return false;
  }
}
Channel::~Channel() { std::cout << "Channel Destroyed" << std::endl; }

// GETTERS
std::string const &Channel::getName(void) const { return (_name); }
std::string const &Channel::getTopic(void) const { return (_topic); }

// SETTER.
void Channel::setTopic(std::string const &topic) { this->_topic = topic; }

// ACTIONS.
void Channel::joinChannel(Client *newClient) {
  // Check if the client is already in the channel

  if (searchClient(newClient, 1) == false) {
    std::cout << "The " << newClient->getName() << " Is already in the Channel"
              << std::endl;
    return;
  }
  // Check if the client is in the invite list
  if (searchClient(newClient, 2) == false) {
    channelUsers.push_back(newClient);
    std::cout << " join the Channel " << newClient->getName() << std::endl;
    return;
  }
  // If the channel is invite-only, reject the join unless the client is an
  // operator
  if (needInvitation == true && newClient->getIsOperator() == false) {
    std::cout << "Channel is invite-only. " << newClient->getName()
              << " cannot join." << std::endl;
    newClient->sendMessage("You cannot join the channel: invite-only.");
    return;
  }
  if (channelUsers.empty()) {
    std::cout << "First client Ever, you are the boss." << std::endl;
    newClient->setOperator(true);
  }
  channelUsers.push_back(newClient);
  std::cout << newClient->getName() << " joined the Channel." << std::endl;
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

// Commands perform by operators.

void Channel::cmdKick(Client *clientOperator, Client *clientUser) {
  if (clientOperator->getIsOperator() == true)
    leaveChannel(clientUser);
  else {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::cmdInvite(Client *clientOperator, Client *clientInvited) {
  if (clientOperator->getIsOperator() == true) {
    if (searchClient(clientInvited, 1) == false) {
      std::cout << "Client already in the channel" << std::endl;
      return;
    }
    if (searchClient(clientInvited, 2) == true) {
      invitedList.push_back(clientInvited);
      std::cout << "Client " << clientInvited << " has been invited"
                << std::endl;
    } else {
      std::cout << "Client has been already invited to the channel"
                << std::endl;
    }
  } else {
    std::cout << "Only operators can perform this funtion" << std::endl;
  }
}

void Channel::cmdTopic(Client *clientOperator, std::string const &topic) {
  if (clientOperator->getIsOperator() == true)
    setTopic(topic);
  else {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::cmdMode(Client *clientOperator, char mode) {
  switch (mode) {
  case 'i':
    needInvitation = !needInvitation;
    break;
  case 't':
    restricTopic = !restricTopic;
    break;
  case 'k':
    // TODO.
    break;
  case 'o': // give a privilege to and user, or remove
    // TODO
    break;
  case 'l':
    // TODO set max number.
    break;
  default:
    std::cout << "Not mode command found " << std::endl;
  }
}