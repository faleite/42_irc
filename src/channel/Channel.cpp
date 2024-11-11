#include "Channel.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

//_______________________________________: Constructor

Channel::Channel() {
  std::cout << "The Channel < " << " > was created" << std::endl;
}
Channel::Channel(std::string const &name)
    : _needInvitation(false), _needVerification(false), _channelKey(""),
      _restricTopic(false), _active(true), limit(-1), _name(name),
      _topic(name + " interactions") {
  std::cout << "The Channel < " << name << " > was created" << std::endl;
}

//_______________________________________: Destructor

Channel::~Channel() { std::cout << "Channel Destroyed" << std::endl; }

//_______________________________________: Getter.

std::string const &Channel::getName(void) const { return (_name); }
std::string const &Channel::getTopic(void) const { return (_topic); }
bool Channel::getRestrictedTopic(void) { return (_restricTopic); }
int Channel::getLimit(void) { return (limit); }

//_______________________________________: Setter.

void Channel::setTopic(std::string const &topic) { this->_topic = topic; }

void Channel::setLimit(int limit) { this->limit = limit; }

void Channel::setPrivilige(Client *client, bool enable) {
  client->setOperator(enable);
}

//_______________________________________: Search.

bool Channel::isOnList(std::string const &nickName) {
  std::vector<Client *>::iterator it;

  for (it = invitedList.begin(); it != invitedList.end(); ++it) {
    if ((*it)->getNickName() == nickName)
      return true;
  }
  return false;
}
std::vector<Client *> channelUsers;

bool Channel::isOnChannel(const std::string &nickName) {
  std::vector<Client *>::iterator it;

  for (it = channelUsers.begin(); it != channelUsers.end(); ++it) {
    if ((*it)->getNickName() == nickName)
      return true;
  }
  return false;
}

//_______________________________________: Actions.

// Set up the password, and the authentication in the list of the client.

bool Channel::getVerification() { return (_needVerification); }
void Channel::joinChannel(Client *newClient, const std::string &password) {

  //__________________________ Check Limit.
  if (limit > 0 && limit == static_cast<int>(channelUsers.size())) {
    newClient->getMessage(Replies::ERR_CHANNELISFULL(this->_name));
    return ;
  }

  //__________________________ Check is on Channel
  if (isOnChannel(newClient->getNickName())) {
    newClient->getMessage(Replies::ERR_USERONCHANNEL(newClient->getNickName(), this->_name));
    return ;
  }

  //__________________________ Check Channel Invite Only
  if (_needInvitation && !newClient->getIsOperator() &&
      !isOnList(newClient->getNickName())) {
    newClient->getMessage(Replies::ERR_INVITEONLYCHAN(this->_name));
    return ;
  }

  //__________________________ Check Verification
  if (_needVerification && password != _channelKey) {
    newClient->getMessage(Replies::ERR_BADCHANNELKEY(this->_name));
    return ;
  }

  //__________________________ Check First client on channel.
  (channelUsers.empty()) ? newClient->setOperator(true)
                         : newClient->setOperator(false);

  //__________________________ Add the client to the channel.
  channelUsers.push_back(newClient);

  newClient->getMessage(Replies::JOIN_CHANNEL(newClient->getNickName(), newClient->getName(), _name));
  this->updateListUsers(newClient);

}

void Channel::updateListUsers(Client *client) {

  std::string NAMREPLY = Replies::RPL_NAMREPLY(client->getNickName(), _name, "=");

  for (size_t i = 0; i < channelUsers.size(); ++i)
      NAMREPLY += (channelUsers[i]->getIsOperator() ? "@" + channelUsers[i]->getNickName() : channelUsers[i]->getNickName()) + " ";
  NAMREPLY += "\r\n";

  std::string ENDOFNAMES = Replies::RPL_ENDOFNAMES(client->getNickName(), _name);
  
  client->getMessage(NAMREPLY + ENDOFNAMES);
  this->broadcastMessage(NAMREPLY + ENDOFNAMES, client);
}

void Channel::leaveChannel(Client *client) {
  std::vector<Client *>::iterator it =
      std::find(channelUsers.begin(), channelUsers.end(), client);
  if (it != channelUsers.end()) {
    channelUsers.erase(it);
    broadcastMessage((*it)->getNickName() + " Has left the Channel", client);
  }

  // treat when channel gets empty.
  // if (channelUsers.empty())
  //   _active = false;
}

void Channel::broadcastMessage(std::string const &message, Client *sender) {
  for (std::vector<Client *>::iterator it = this->channelUsers.begin();
       it != channelUsers.end(); ++it) {
    if (*it != sender)
      (*it)->getMessage(message);
  }
}

int Channel::getUsers(void) {
  int count = 0;
  for (std::vector<Client *>::iterator it = channelUsers.begin();
       it != channelUsers.end(); ++it) {
    count++;
  }
  return count;
}

//_______________________________________: Perform Operators.

void Channel::changeTopic(Client *clientOperator, std::string const &topic) {
  if (clientOperator->getIsOperator() == true)
    setTopic(topic);
  else {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::invite(std::string const &inviter, Client *clientInvited) {
  if (isOnChannel(clientInvited->getNickName())) {
    std::cout << "Client already in the channel" << std::endl;
    return;
  }
  if (isOnList(clientInvited->getNickName()) == false) {
    invitedList.push_back(clientInvited);
    clientInvited->getMessage(Replies::RPL_INVITING("INVITE", inviter, _name));
  } else {
    std::cout << "Client has been already invited to the channel" << std::endl;
  }
  for (int i = 0; i < (int) invitedList.size(); ++i)
    std::cout << invitedList[i]->getNickName() << std::endl;
}

// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params) {

  if (!isOnChannel(clientOperator->getNickName()))
    throw(std::runtime_error("You are not part of this channel"));

  //__________________________ IF IS + WILL ADD THIS MODE OF THE CHANNEL.
  bool enable;
  modeCmd[0] == '+' ? enable = true : enable = false;

  for (int i = 1; modeCmd[i]; ++i) {

    switch (modeCmd[i]) {
    //_______________________________________ INVITATION MODE.
    case 'i':
      _needInvitation = enable;
      broadcastMessage(clientOperator->getName() +
                       " : set the channel to invitation mode only", clientOperator);
      break;

    //_______________________________________ LIMIT USER NAME MODE.
    case 'l':
      if (enable) {
        if (params.size() >= 3) {
          if (stringToInt(params[2], limit)) {
            broadcastMessage(clientOperator->getName() +
                             " : set the channel limit to" + params[2], clientOperator);
          } else {
            clientOperator->getMessage("Wrong Limit ");
          }
        } else {
          std::cout << "Error: Missing user limit parameter."
                    << std::endl; // Remove for invalid.
        }
      } else {
        limit = -1;
        broadcastMessage(clientOperator->getName() +
                         " : delete the channel limit ", clientOperator);
      }
      break;
    //____________________________ Restricted topic.
    case 't':
      _restricTopic = enable;
      std::cout << "Restricted Topic Mode " << (enable ? "Enable" : "Disable")
                << std::endl;
      break;

    //______________________________ GIVE PRIVILEGES
    case 'o':
      // 1. find the client nick.
      for (std::vector<Client *>::iterator iter = channelUsers.begin();
           iter != channelUsers.end(); iter++) {
        (*iter)->getNickName() == params[2]
            ? setPrivilige((*iter), enable)
            : clientOperator->getMessage("User not found");
      }
      break;
    case 'k':
      _needVerification = enable;
      if (enable) {
        if (params.size() >= 3) {
          _channelKey = params[2];
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
