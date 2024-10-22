#include "Channel.hpp"
#include <stdexcept>
#include <string>
#include <vector>

//_______________________________________: Constructor

Channel::Channel(std::string const &name)
    : needInvitation(false), needVerification(false), _channelKey(""),
      restricTopic(false), active(true), _name(name), _topic("") {
  std::cout << "The Channel < " << name << " > was created" << std::endl;
}

//_______________________________________: Destructor

Channel::~Channel() { std::cout << "Channel Destroyed" << std::endl; }

//_______________________________________: Getter.

std::string const &Channel::getName(void) const { return (_name); }
std::string const &Channel::getTopic(void) const { return (_topic); }
int const Channel::getLimit(void) const { return (limit); }

//_______________________________________: Setter.

void Channel::setTopic(std::string const &topic) { this->_topic = topic; }

void Channel::setLimit(int limit) { this->limit = limit; }

void Channel::setPrivilige(Client *client, bool enable) {
  client->setOperator(enable);
}

//_______________________________________: Search.

bool Channel::isOnList(Client *client) {
  std::vector<Client *>::iterator iter;

  iter = std::find(invitedList.begin(), invitedList.end(), client);
  if (iter == invitedList.end())
    return false;
  return false;
}

bool Channel::isOnChannel(Client *client) {
  std::vector<Client *>::iterator iter;

  iter = std::find(channelUsers.begin(), channelUsers.end(), client);
  if (iter == channelUsers.end())
    return false;
  return true;
}
//_______________________________________: Actions.

void Channel::joinChannel(Client *newClient) {
  // Check if the client is already in the channel
  if (isOnChannel(newClient) == true) {
    std::cout << "The " << newClient->getName() << " Is already in the Channel"
              << std::endl;
    return;
  }
  // Check if the client is in the invite list
  if (isOnList(newClient) == true) {
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

//_______________________________________: Perform Operators.

void Channel::kick(Client *clientOperator, Client *clientUser) {
  if (clientOperator->getIsOperator() == true)
    leaveChannel(clientUser);
  else {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::changeTopic(Client *clientOperator, std::string const &topic) {
  if (clientOperator->getIsOperator() == true)
    setTopic(topic);
  else {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::invite(Client *clientOperator, Client *clientInvited) {
  if (clientOperator->getIsOperator() == true) {
    if (isOnChannel(clientInvited) == true) {
      std::cout << "Client already in the channel" << std::endl;
      return;
    }
    if (isOnList(clientInvited) == false) {
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

// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params) {

  bool enable;

  if (clientOperator->getIsOperator() == false) {
    std::cout << "Only operators can perform this funtion" << std::endl;
    return;
  }
  modeCmd[0] == '+' ? enable = true : enable = false;
  size_t modeParameters = 0;

  for (int i = 1; modeCmd[i]; ++i) {

    switch (modeCmd[i]) {
    case 'i':
      needInvitation = enable;
      std::cout << "Invite Only Mode" << (enable ? "Enable" : "Disable")
                << std::endl;
      break;
    case 't':
      restricTopic = enable;
      std::cout << "Restricted Topic Mode " << (enable ? "Enable" : "Disable")
                << std::endl;
      break;
    case 'l':
      if (enable) {
        if (modeParameters < params.size()) {
          try {
            limit = std::stoi(params[modeParameters++]);
            std::cout << "User limit set to: " << limit << std::endl;
          } catch (std::invalid_argument &) {
            std::cout << "Error: Invalid user limit parameter." << std::endl;
          }
        } else {
          std::cout << "Error: Missing user limit parameter." << std::endl;
        }
      } else {
        limit = -1;
        std::cout << "Limit parameter Removed." << std::endl;
      }
      // TODO.
      break;
    case 'o': // give a privilege to and user, or remove
      setPrivilige(clientOperator, enable);
      break;
    case 'k':
      needVerification = enable;
      if (enable) {
        if (modeParameters < params.size()) {
          _channelKey = params[modeParameters++];
          std::cout << "Channel key set to: " << _channelKey << std::endl;
        } else {
          std::cout << "Missing Channel key Parameter " << std::endl;
        }
      } else {
        _channelKey.clear();
        std::cout << "Channel key Removed " << std::endl;
      }
      break;
    default:
      std::cout << "Not mode command found " << std::endl;
    }
  }
}
