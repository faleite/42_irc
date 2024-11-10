#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "../channel/Channel.hpp"
#include "../server/Server.hpp"
#include <string>

class Replies {
public:
  static std::string ERR_PASSWDMISMATCH();
  static std::string ERR_NOTREGISTERED();
  static std::string ERR_ALREADYREGISTRED();
  static std::string ERR_NEEDMOREPARAMS(const std::string &cmd);
  static std::string ERR_UNKNOWNCOMMAND(const std::string &cmd);
  static std::string ERR_NICKNAMEINUSE(const std::string &nick);
  static std::string ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel);
  static std::string LEAVE_CHANNEL(const std::string &nick, 
                                   const std::string &user,
                                   const std::string &channel);
  static std::string RPL_WELCOME(const std::string &nick,
                                 const std::string &user,
                                 const std::string &channel_name);
  static std::string JOIN_CHANNEL(const std::string &nick,
                                 const std::string &user,
                                 const std::string &channel_name);
  static std::string RPL_NAMREPLY(const std::string &nick,
                                 const std::string &channel,
                                 const std::string &type);
  static std::string RPL_ENDOFNAMES(const std::string &nick,
                                 const std::string &channel_name);
  static std::string RPL_CHANNELMODEIS(const std::string &nick,
                                 const std::string &channel_name,
                                 const std::string &modes);
  static std::string ERR_CHANNELISFULL(const std::string &channel);
  static std::string ERR_INVITEONLYCHAN(const std::string &channel);
  static std::string ERR_BADCHANNELKEY(const std::string &channel);
  static std::string ERR_USERONCHANNEL(const std::string &nick, const std::string &channel);
  static std::string ERR_NOSUCHNICK(const std::string &nick);
};

#endif // REPLIES_HPP