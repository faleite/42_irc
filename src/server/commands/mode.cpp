#include "../Server.hpp"
// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params)
{

  if (!isOnChannel(clientOperator->getNickName()))
    throw(std::runtime_error("You are not part of this channel"));

  //__________________________ IF IS + WILL ADD THIS MODE OF THE CHANNEL.
  bool enable;
  modeCmd[0] == '+' ? enable = true : enable = false;

  for (int i = 1; modeCmd[i]; ++i)
  {

    switch (modeCmd[i])
    {
    //_______________________________________ INVITATION MODE.
    case 'i':
      _needInvitation = enable;
      broadcastMessage(clientOperator->getName() +
                           " : set the channel to invitation mode only",
                       clientOperator);
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
                                 " : set the channel limit to" + params[2],
                             clientOperator);
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
                             " : delete the channel limit ",
                         clientOperator);
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
           iter != channelUsers.end(); iter++)
      {
        isOnChannel((*iter)->getNickName())
            ? setPrivilige((*iter), enable)
            : clientOperator->getMessage("User not found");
      }
      break;
    case 'k':
      _needVerification = enable;
      if (enable)
      {
        if (params.size() >= 3)
        {
          _channelKey = params[2];
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