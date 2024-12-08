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
  static std::string ERR_NOSUCHCHANNEL(const std::string &nick,
                                       const std::string &channel);
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
  static std::string ERR_USERONCHANNEL(const std::string &nick,
                                       const std::string &channel);
  static std::string ERR_NOSUCHNICK(const std::string &nick);
  static std::string ERR_CHANOPRIVSNEEDED(const std::string &channel);
  static std::string ERR_NOTONCHANNEL(const std::string &channel);
  static std::string KICK_USER(const std::string &cmd, const std::string &nick,
                               const std::string &name,
                               const std::string &channel,
                               const std::string &target,
                               const std::string &reason);
  static std::string PART_USER(const std::string &cmd, const std::string &nick,
                               const std::string &name,
                               const std::string &channel,
                               const std::string &reason);
  static std::string RPL_INVITING(const std::string &cmd,
                                  const std::string &nick,
                                  const std::string &channel);

  static std::string ERR_USERNOTINCHANNEL(const std::string &nick,
                                          const std::string &channel,
                                          const std::string &nickTarget);
  static std::string RPL_TOPIC(const std::string &channel, const std::string &topic);
  static std::string RPL_NOTOPIC(const std::string &channel);

};

#endif // REPLIES_HPP