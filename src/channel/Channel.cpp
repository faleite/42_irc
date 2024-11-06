#include "Channel.hpp"
#include <stdexcept>
#include <string>
#include <vector>

//_______________________________________: Constructor

Channel::Channel()
{
  std::cout << "The Channel < " << " > was created" << std::endl;
}
Channel::Channel(std::string const &name)
    : _needInvitation(false), _needVerification(false), _channelKey(""),
      _restricTopic(false), _active(true), limit(-1), _name(name),
      _topic(name + " interactions")
{
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

void Channel::setPrivilige(Client *client, bool enable)
{
  client->setOperator(enable);
}

//_______________________________________: Search.

bool Channel::isOnList(Client *client)
{
  std::vector<Client *>::iterator iter;

  for (iter = channelUsers.begin(); iter != channelUsers.end(); iter++)
    if ((*iter)->getNickName() == client->getNickName())
      return (true);
  return (false);
}

bool Channel::isOnChannel(Client *client)
{
  std::vector<Client *>::iterator iter;

  iter = std::find(channelUsers.begin(), channelUsers.end(), client);
  if (iter == channelUsers.end())
    return false;
  return true;
}

//_______________________________________: Actions.

// Set up the password, and the authentication in the list of the client.

bool Channel::getVerification() { return (_needVerification); }
void Channel::joinChannel(Client *newClient, const std::string &password)
{

  //__________________________ Check Limit.
  if (limit > 0 && limit == static_cast<int>(channelUsers.size()))
  {
    throw std::runtime_error(
        "You can not join to the Channel, max set user reached");
  }

  //__________________________ Check is on Channel
  if (isOnChannel(newClient))
  {
    throw std::runtime_error("You are already in the channel");
  }

  //__________________________ Check Channel Invite Only
  if (_needInvitation && !newClient->getIsOperator() && !isOnList(newClient))
  {
    throw std::runtime_error("You cannot join the channel: invite-only.");
  }

  //__________________________ Check Verification
  if (_needVerification && password != _channelKey)
  {
    throw std::runtime_error(
        "You cannot join the channel: use correct password.");
  }

  //__________________________ Check First client on channel.
  (channelUsers.empty()) ? newClient->setOperator(true)
                         : newClient->setOperator(false);

  //__________________________ Add the client to the channel.
  channelUsers.push_back(newClient);
  this->broadcastMessage(newClient->getNickName() + " joined the Channel." +
                         _name);
  Replies::RPL_WELCOME(newClient->getNickName(), newClient->getName(), _name);
  // newClient->getMessage(":juan!~u@e77ncepu88yiy.irc JOIN #local");
  // newClient->getMessage(Replies::RPL_NAMREPLY());
}

void Channel::leaveChannel(Client *client)
{
  std::vector<Client *>::iterator it;
  it = std::find(channelUsers.begin(), channelUsers.end(), client);
  if (it != channelUsers.end())
  {
    channelUsers.erase(it);
    std::cout << client->getName() << " Has left the Channel" << std::endl;
  }
  if (channelUsers.empty())
    _active = false;
}

void Channel::broadcastMessage(std::string const &message)
{
  for (std::vector<Client *>::iterator it = channelUsers.begin();
       it != channelUsers.end(); ++it)
  {
    (*it)->getMessage(message);
  }
}

int Channel::getUsers(void)
{
  int count = 0;
  for (std::vector<Client *>::iterator it = channelUsers.begin();
       it != channelUsers.end(); ++it)
  {
    count++;
  }
  return count;
}

//_______________________________________: Perform Operators.

void Channel::kick(Client *clientOperator, Client *clientUser)
{
  if (clientOperator->getIsOperator() == true)
    leaveChannel(clientUser);
  else
  {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::changeTopic(Client *clientOperator, std::string const &topic)
{
  if (clientOperator->getIsOperator() == true)
    setTopic(topic);
  else
  {
    std::cout << "Just the User Operator can perform this function"
              << std::endl;
  }
}

void Channel::invite(Client *clientInvited)
{
  if (isOnChannel(clientInvited))
  {
    std::cout << "Client already in the channel" << std::endl;
    return;
  }
  if (isOnList(clientInvited) == false)
  {
    invitedList.push_back(clientInvited);
    std::cout << "Client " << clientInvited->getNickName() << " has been invited"
              << std::endl;
  }
  else
  {
    std::cout << "Client has been already invited to the channel"
              << std::endl;
  }
}

// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params)
{

  if (!isOnChannel(clientOperator))
    throw(std::runtime_error("You are not part of this channel"));

  //__________________________ IF IS + WILL ADD THIS MODE OF THE CHANNEL.
  if (!isOnChannel(clientOperator))
    throw(std::runtime_error("You are not part of this channel"));

  //__________________________ IF IS + WILL ADD THIS MODE OF THE CHANNEL.
  bool enable;
  modeCmd[0] == '+' ? enable = true : enable = false;
  size_t modeParameters = 0;

  for (int i = 1; modeCmd[i]; ++i)
  {

    switch (modeCmd[i])
    {
    //_______________________________________ INVITATION MODE.
    case 'i':
      _needInvitation = enable;
      broadcastMessage(clientOperator->getName() +
                       " : set the channel to invitation mode only");
      break;

    //_______________________________________ LIMIT USER NAME MODE.
    case 'l':
      if (enable)
      {
        if (params.size() >= 3)
        {
          if (stringToInt(params[2], limit))
          {
            broadcastMessage(clientOperator->getName() +
                             " : set the channel limit to" + params[2]);
          }
          else
          {
            clientOperator->getMessage("Wrong Limit ");
          }
        }
        else
        {
          std::cout << "Error: Missing user limit parameter."
                    << std::endl; // Remove for invalid.
        }
      }
      else
      {
        limit = -1;
        broadcastMessage(clientOperator->getName() +
                         " : delete the channel limit ");
      }
      break;
    //____________________________ Restricted topic.
    case 't':
      _restricTopic = enable;
      std::cout << "Restricted Topic Mode " << (enable ? "Enable" : "Disable")
                << std::endl;
      break;
    case 'o': // give a privilege to and user, or remove
      // 1. find the client nick.
      for (std::vector<Client *>::iterator iter = channelUsers.begin();
           iter != channelUsers.end(); iter++)
      {
        (*iter)->getNickName() == params[2] ? setPrivilige((*iter), enable)
                                            : clientOperator->getMessage("User not found");
      }
      break;
    case 'k':
      _needVerification = enable;
      if (enable)
      {
        if (modeParameters < params.size())
        {
          _channelKey = params[modeParameters++];
          std::cout << "Channel key set to: " << _channelKey << std::endl;
        }
        else
        {
          std::cout << "Missing Channel key Parameter " << std::endl;
        }
      }
      else
      {
        _channelKey.clear();
        std::cout << "Channel key Removed " << std::endl;
      }
      break;
    default:
      std::cout << "Not mode command found " << std::endl;
    }
  }
}
