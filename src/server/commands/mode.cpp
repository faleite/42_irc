#include "../Server.hpp"
// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params) {
  std::string message0;
  std::string message1;

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
                           " : set the channel to invitation mode only",
                       clientOperator);
      break;

    //_______________________________________ LIMIT USER NAME MODE.
    case 'l':
      if (enable) {
        if (params.size() >= 3) {
          if (stringToInt(params[2], limit)) {
            broadcastMessage(clientOperator->getName() +
                                 " : set the channel limit to" + params[2],
                             clientOperator);
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
                             " : delete the channel limit ",
                         clientOperator);
      }
      break;
    //____________________________ Restricted topic.
    case 't':
      //____________________________________ ADD to the Replies after test .
      _restricTopic = enable;
      message0 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                " :Topic restricted mode set\r\n";
      message1 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                " :Topic restricted mode set\r\n";        
      enable ? broadcastMessage(":jf.irc 324 " + clientOperator->getNickName() +
                                    " " + _name +
                                    " :Topic restricted mode set\r\n",
                                clientOperator)
             : broadcastMessage(":jf.irc 324 " + clientOperator->getNickName() +
                                    " " + _name +
                                    " :Topic restricted mode unset\r\n",
                                clientOperator);
      break;

    //______________________________ GIVE PRIVILEGES
    case 'o':
      // 1. find the client nick.
      for (std::vector<Client *>::iterator iter = channelUsers.begin();
           iter != channelUsers.end(); iter++) {
        isOnChannel((*iter)->getNickName())
            ? setPrivilige((*iter), enable)
            : clientOperator->getMessage("User not found");
      }
      break;
    case 'k':
      _needVerification = enable;
      if (enable) {
        if (params.size() >= 3) {
          _channelKey = params[2];
          std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                                " " + _name + " :Password set\r\n";
          broadcastMessage(message, clientOperator);
        } else {
          std::string message = ":jf.irc 461 " + clientOperator->getNickName() +
                                " " + _name + " :Not enough parameters\r\n";
          clientOperator->getMessage(message);
          clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("MODE +k"));
        }
      } else {
        _channelKey.clear();
        std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                              " " + _name + " :Password removed\r\n";
        broadcastMessage(message, clientOperator);
      }
      break;
    default:
      std::cout << "Not mode command found " << std::endl;
    }
  }
}