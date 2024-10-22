#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../client/Client.hpp"
#include <string>


class Channel {

private:
  std::string _name;
  std::string _topic;
  std::vector<Client *> channelUsers;

public:
  Channel(std::string const &name);
  ~Channel();
  // getters.
  std::string const getName(void) ;
  std::string const getTopic(void);
  // Setter.
  void setTopic(std::string const &topic);
  // Actions.
  void joinChannel(Client *newClient) ;
  void leaveChannel(Client *client) ;
  void const brodcastMessage(std::string &message) const;
};
#endif