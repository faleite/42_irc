#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../client/Client.hpp"
#include <string>

class Channel {

private:
  bool needInvitation;
  bool needVerification;
  bool restricTopic;
  bool active;
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
  int const getLimit(void) const;
  // Setter.
  void setTopic(std::string const &topic);
  void setLimit(int limit);
  void setPrivilige(Client *client, bool enable);

  // Actions.
  void joinChannel(Client *newClient);
  void leaveChannel(Client *client);
  void brodcastMessage(std::string &message);
  bool isOnList(Client *client);
  bool isOnChannel(Client *client);
  void changeTopic(Client *clientOperator, std::string const &topic);

  // Commands perform by operators.
  void kick(Client *clientOperator, Client *clientUser);
  void mode(Client *clientOperator, std::string const &mode);
  void modeLimit(int limit, bool enable);
  void invite(Client *clientOperator, Client *ClientInvited);
  void setTopic(Client *clientOperator, std::string const &newTopic);
};
#endif