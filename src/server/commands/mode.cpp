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
      message0 = ":server 324 " + clientOperator->getNickName() + " " + _name +
                 " :Invite only mode set\r\n";
      message1 = ":server 324 " + clientOperator->getNickName() + " " + _name +
                 " :Invite only mode unset\r\n";
      enable ? (broadcastMessage(message0, clientOperator),
                clientOperator->getMessage(message0))
             : (broadcastMessage(message1, clientOperator),
                clientOperator->getMessage(message1));
      break;

    //_______________________________________ LIMIT USER NAME MODE.
    case 'l':
      if (enable) {
        if (params.size() >= 3) {
          if (stringToInt(params[2], limit)) {
            std::string message = ":server 324 " +
                                  clientOperator->getNickName() + " " + _name +
                                  " :User limit set to " + params[2] + "\r\n";

            broadcastMessage(message, clientOperator);
            clientOperator->getMessage(message);
          } else {
            clientOperator->getMessage("Wrong Limit ");
          }
        } else {
          clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("Mode +l"));
        }
      } else {
        limit = -1;
        std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                              " " + _name + " :User limit removed\r\n";

        broadcastMessage(message, clientOperator);
      }
      break;
    //____________________________ Restricted topic.
    case 't':
      //____________________________________ ADD to the Replies after test .
      _restricTopic = enable;
      message0 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Topic restricted mode set\r\n";
      message1 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Topic restricted mode unset\r\n";
      enable ? (broadcastMessage(message0, clientOperator),
                clientOperator->getMessage(message0))
             : (broadcastMessage(message1, clientOperator),
                clientOperator->getMessage(message1));
      break;

    //______________________________ GIVE PRIVILEGES
    case 'o':
      // 1. find the client nick.
      if (params.size() >= 3) {

        for (std::vector<Client *>::iterator iter = channelUsers.begin();
             iter != channelUsers.end(); iter++) {
          if ((*iter)->getNickName() == params[2]) {
            setPrivilige((*iter), enable);
            std::string message =
                ":jf.irc 381 " + clientOperator->getNickName() + " " + _name +
                " :Operator privileges given to " + params[2] + "\r\n";
            break;
          }
        }
        clientOperator->getMessage(Replies::ERR_USERNOTINCHANNEL(
            clientOperator->getNickName(), params[0], params[2]));
      } else {
        clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("mode +o"));
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