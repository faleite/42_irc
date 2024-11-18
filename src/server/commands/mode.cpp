#include "../Server.hpp"
// void Channel::modeLimit(int limit, bool enable) {}
void Channel::mode(Client *clientOperator, std::string const &modeCmd,
                   std::vector<std::string> params)
{
  std::string message0;
  std::string message1;
  bool isOnChan = false;

  // if (!isOnChannel(clientOperator->getNickName()))
  //   throw(std::runtime_error("You are not part of this channel"));

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
      message0 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Invite only mode set";
      message1 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Invite only mode unset";
      enable ? (broadcastMessage(message0, clientOperator),
                clientOperator->getMessage(message0))
             : (broadcastMessage(message1, clientOperator),
                clientOperator->getMessage(message1));
      break;

    //_______________________________________ LIMIT USER NAME MODE.
    case 'l':
      if (enable)
      {
        if (params.size() >= 3)
        {
          if (stringToInt(params[2], limit))
          {
            std::string message = ":server 324 " +
                                  clientOperator->getNickName() + " " + _name +
                                  " :User limit set to " + params[2];

            broadcastMessage(message, clientOperator);
            clientOperator->getMessage(message);
          }
          else
          {
            clientOperator->getMessage("Wrong Limit ");
          }
        }
        else
        {
          clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("Mode +l"));
        }
      }
      else
      {
        limit = -1;
        std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                              " " + _name + " :User limit removed";

        broadcastMessage(message, clientOperator);
      }
      break;
    //____________________________ Restricted topic.
    case 't':
      //____________________________________ ADD to the Replies after test .
      _restricTopic = enable;
      message0 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Topic restricted mode set";
      message1 = ":jf.irc 324 " + clientOperator->getNickName() + " " + _name +
                 " :Topic restricted mode unset";
      enable ? (broadcastMessage(message0, clientOperator),
                clientOperator->getMessage(message0))
             : (broadcastMessage(message1, clientOperator),
                clientOperator->getMessage(message1));
      break;

    //______________________________ GIVE PRIVILEGES
    case 'o':
      // 1. find the client nick.
      if (params.size() >= 3)
      {

        for (std::vector<Client *>::iterator iter = channelUsers.begin();
             iter != channelUsers.end(); iter++)
        {
          if ((*iter)->getNickName() == params[2])
          {
            isOnChan = true;
            setPrivilige((*iter), enable);
            std::string message =
                ":jf.irc 381 " + clientOperator->getNickName() + " " + _name +
                " :Operator privileges given to " + params[2];
            // std::string message1 = ":You are now an IRC operator";

            std::string message2 =
                ":jf.irc 381 " + clientOperator->getNickName() + " " + _name +
                " :Operator privileges removed to " + params[2];
            // std::string message3 = ":You are not more an IRC operator";
            enable ? (broadcastMessage(message, clientOperator),
                      clientOperator->getMessage(message))
                      // (*iter)->getMessage(message1))
                   : (broadcastMessage(message2, clientOperator),
                      clientOperator->getMessage(message2));
                      // (*iter)->getMessage(message3));
            break;
          }
        }
        if (isOnChan == false)
          clientOperator->getMessage(Replies::ERR_USERNOTINCHANNEL(
              clientOperator->getNickName(), params[0], params[2]));
      }
      else
      {
        clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("mode +o"));
      }
      break;
    case 'k':
      _needVerification = enable;
      if (enable)
      {
        if (params.size() >= 3)
        {
          _channelKey = params[2];
          std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                                " " + _name + " :Password set";
          broadcastMessage(message, clientOperator);
          clientOperator->getMessage(message);
        }
        else
        {
          std::string message = ":jf.irc 461 " + clientOperator->getNickName() +
                                " " + _name + " :Not enough parameters";
          clientOperator->getMessage(message);
          clientOperator->getMessage(Replies::ERR_NEEDMOREPARAMS("MODE +k"));
        }
      }
      else
      {
        _channelKey.clear();
        std::string message = ":jf.irc 324 " + clientOperator->getNickName() +
                              " " + _name + " :Password removed";
        broadcastMessage(message, clientOperator);
      }
      break;
    default:
      std::cout << "Not mode command found " << std::endl;
    }
  }
}