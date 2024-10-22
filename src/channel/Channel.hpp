#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../client/Client.hpp"
#include <string>

class Channel {

private:
  bool active;
  std::string _name;
  std::string _topic;
  std::vector<Client *> channelUsers;

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
};
#endif