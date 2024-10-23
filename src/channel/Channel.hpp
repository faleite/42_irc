#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../client/Client.hpp"
#include <string>
#include <sstream>

class Channel {

private:
  bool _needInvitation;
  bool _needVerification;
  std::string _channelKey;
  bool _restricTopic;
  bool _active;
  int limit;
  std::string _name;
  std::string _topic;
  std::vector<Client *> channelUsers;
  std::vector<Client *> invitedList;

public:
  Channel(std::string const &name);
  ~Channel();
  // getters.
  std::string const &getName(void) const;
  std::string const &getTopic(void) const;
  int  getLimit(void) ;
  // Setter.
  void setTopic(std::string const &topic);
  void setLimit(int limit);
  void setPrivilige(Client *client, bool enable);

  // Actions.
  void joinChannel(Client *newClient, const std::string &password);
  void leaveChannel(Client *client);
  void brodcastMessage(std::string &message);
  bool isOnList(Client *client);
  bool isOnChannel(Client *client);
  void changeTopic(Client *clientOperator, std::string const &topic);

  // Commands perform by operators.
  void kick(Client *clientOperator, Client *clientUser);
  void mode(Client *clientOperator, std::string const &mode,
            std::vector<std::string> params);
  // void modeLimit(int limit, bool enable, std::vector<std::string> params);
  void invite(Client *clientOperator, Client *ClientInvited);
  void setTopic(Client *clientOperator, std::string const &newTopic);
};
#endif