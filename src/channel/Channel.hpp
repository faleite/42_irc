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
  // Setter.
  void setTopic(std::string const &topic);
  // Actions.
  void joinChannel(Client *newClient);
  void leaveChannel(Client *client);
  void brodcastMessage(std::string &message);
  bool searchClient(Client *client, int use);

  // Commands perform by operators.
  void cmdKick(Client *clientOperator, Client *clientUser);
  void cmdMode(Client *clientOperator, char mode);
  void cmdInvite(Client *clientOperator, Client *ClientInvited);
  void cmdTopic(Client *clientOperator, std::string const &newTopic);
};
#endif