#include "Replies.hpp"

// check for the channel replies
/*
       001    RPL_WELCOME
              "Welcome to the Internet Relay Network
               <nick>!<user>@<host>"
       325    RPL_UNIQOPIS
              "<channel> <nickname>"

       324    RPL_CHANNELMODEIS
              "<channel> <mode> <mode params>"

       331    RPL_NOTOPIC
              "<channel> :No topic is set"
       332    RPL_TOPIC
              "<channel> :<topic>"

         - When sending a TOPIC message to determine the
           channel topic, one of two replies is sent.  If
           the topic is set, RPL_TOPIC is sent back else
           RPL_NOTOPIC.

       341    RPL_INVITING
              "<channel> <nick>"

         - Returned by the server to indicate that the
           attempted INVITE message was successful and is
           being passed onto the end client.

       342    RPL_SUMMONING
              "<user> :Summoning user to IRC"

         - Returned by a server answering a SUMMON message to
           indicate that it is summoning that user.

       346    RPL_INVITELIST
              "<channel> <invitemask>"
       347    RPL_ENDOFINVITELIST
              "<channel> :End of channel invite list"

         - When listing the 'invitations masks' for a given channel,
           a server is required to send the list back using the
           RPL_INVITELIST and RPL_ENDOFINVITELIST messages.  A
           separate RPL_INVITELIST is sent for each active mask.
           After the masks have been listed (or if none present) a
           RPL_ENDOFINVITELIST MUST be sent.

       348    RPL_EXCEPTLIST
              "<channel> <exceptionmask>"
       349    RPL_ENDOFEXCEPTLIST
              "<channel> :End of channel exception list"


        353   RPL_NAMREPLY
              "( "=" / "*" / "@" ) <channel>
               :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
         - "@" is used for secret channels, "*" for private
           channels, and "=" for others (public channels).
              */

std::string Replies::ERR_NEEDMOREPARAMS(const std::string &cmd) {
  return (":jf.irc 461 * " + cmd + " :Not enough parameters");
}

std::string Replies::ERR_UNKNOWNCOMMAND(const std::string &cmd) {
  return (":jf.irc 421 * " + cmd + " :Unknown command");
}

std::string Replies::ERR_PASSWDMISMATCH() {
  return (":jf.irc 464 * :Password incorrect");
}

std::string Replies::ERR_NOTREGISTERED() {
  return (":jf.irc 451 * :You need to register before you can use that "
          "command"); // ":You have not registered"
}

std::string Replies::ERR_ALREADYREGISTRED() {
  return (":jf.irc 462 * :You may not reregisterd");
}

std::string Replies::ERR_NICKNAMEINUSE(const std::string &nick) {
  return (":jf.irc 433 * " + nick + " :Nickname is already in use");
}

// 001.
std::string Replies::RPL_WELCOME(const std::string &nick,
                                 const std::string &user,
                                 const std::string &channel_name) {
  // Opcion 1.
  return std::string(":") + nick + "!" + user + "@localhoost" + " JOIN " +
         channel_name + " * :" + user;

  // Opcion 2.
  // return std::string(":jf.irc 433") + nick + "!" + user + "@localhoost" +
  //        " JOIN " + channel_name + " * :" + user + "\r\n";
}

/**
401    ERR_NOSUCHNICK
              "<nickname> :No such nick/channel"

          - Used to indicate the nickname parameter supplied to a
            command is currently unused.
*/
std::string Replies::ERR_NOSUCHNICK(const std::string &nick) {
  return (":jf.irc 401 " + nick + " :No such nick/channel");
}

std::string Replies::ERR_NOSUCHCHANNEL(const std::string &nick,
                                       const std::string &channel) {
  return (":jf.irc 403 " + nick + " " + channel + " :No such channel");
}

/**
471  ERR_CHANNELISFULL
    "<channel> :Cannot join channel (+l)"
*/
std::string Replies::ERR_CHANNELISFULL(const std::string &channel) {
  return (":jf.irc 471 " + channel + " :Cannot join channel (+l)");
}

/**
473    ERR_INVITEONLYCHAN
              "<channel> :Cannot join channel (+i)"
*/
std::string Replies::ERR_INVITEONLYCHAN(const std::string &channel) {
  return (":jf.irc 473 " + channel + " :Cannot join channel (+i)");
}

/**
475    ERR_BADCHANNELKEY
              "<channel> :Cannot join channel (+k)"
*/
std::string Replies::ERR_BADCHANNELKEY(const std::string &channel) {
  return (":jf.irc 475 " + channel + " :Cannot join channel (+k)");
}

/**
  443    ERR_USERONCHANNEL
              "<user> <channel> :is already on channel"

         - Returned when a client tries to invite a user to a
           channel they are already on.
*/
std::string Replies::ERR_USERONCHANNEL(const std::string &nick,
                                       const std::string &channel) {
  return (":jf.irc 443 " + nick + " " + channel + " is already on channel");
}

std::string Replies::LEAVE_CHANNEL(const std::string &nick,
                                   const std::string &user,
                                   const std::string &channel) {
  return std::string(":" + nick + "!" + user + "@localhoost" + " PART " +
                     channel);
}

std::string Replies::JOIN_CHANNEL(const std::string &nick,
                                  const std::string &user,
                                  const std::string &channel_name) {
  return std::string(":") + nick + "!" + user + "@localhoost" + " JOIN " +
         channel_name + " * realname";
}

/*
 * 353    RPL_NAMREPLY
 *               "( "=" / "*" / "@" ) <channel>
 *                :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
 *          - "@" is used for secret channels, "*" for private
 *            channels, and "=" for others (public channels).
 */
std::string
Replies::RPL_NAMREPLY(const std::string &nick, const std::string &channel,
                      const std::string &type) { // = (public (default))
  // return std::string(":jf.irc 353 " + nick + type + channel + " :@" + nick +
  // "!" + is_operator + "@localhoost"); return std::string(":jf.irc 353 " +
  // nick + " " + type + " " + channel + " :" + is_operator + nick);
  return std::string(":jf.irc 353 " + nick + " " + type + " " + channel + " :");
}

/**
366    RPL_ENDOFNAMES
              "<channel> :End of NAMES list"

         - To reply to a NAMES message, a reply pair consisting
           of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
           server back to the client.  If there is no channel
           found as in the query, then only RPL_ENDOFNAMES is
           returned.  The exception to this is when a NAMES
           message is sent with no parameters and all visible
           channels and contents are sent back in a series of
           RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
           the end.
*/
std::string Replies::RPL_ENDOFNAMES(const std::string &nick,
                                    const std::string &channel_name) {
  return std::string(":jf.irc 366 " + nick + " " + channel_name +
                     " :End of NAMES list.");
}

/**

  324    RPL_CHANNELMODEIS
              "<channel> <mode> <mode params>"
    Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.

      The available modes are as follows:

           a - user is flagged as away;
           i - marks a users as invisible;
           w - user receives wallops;
           r - restricted user connection;
           o - operator flag;
           O - local operator flag;
           s - marks a user for receipt of server notices.

 */
std::string Replies::RPL_CHANNELMODEIS(const std::string &nick,
                                       const std::string &channel_name,
                                       const std::string &modes) {
  return std::string(":jf.irc 324 " + nick + " " + channel_name + " " + modes);
}

// 4.2.8 Kick command

//       Command: KICK
//    Parameters: <channel> <user> [<comment>]

//    The KICK command can be  used  to  forcibly  remove  a  user  from  a
//    channel.   It  'kicks  them  out'  of the channel (forced PART).

// Oikarinen & Reed                                               [Page 25]
//
// RFC 1459              Internet Relay Chat Protocol              May 1993

//    Only a channel operator may kick another user out of a  channel.
//    Each  server that  receives  a KICK message checks that it is valid
//    (ie the sender is actually a  channel  operator)  before  removing
//    the  victim  from  the channel.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS    yes          ERR_NOSUCHCHANNEL yes
//            ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//            ERR_NOTONCHANNEL

//    Examples:

// KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

// KICK #Finnish John :Speaking English
//                                 ; Kick John from #Finnish using
//                                 "Speaking English" as the reason
//                                 (comment).

// :WiZ KICK #Finnish John         ; KICK message from WiZ to remove John
//                                 from channel #Finnish

// NOTE:
//      It is possible to extend the KICK command parameters to the
// following:

// 482     ERR_CHANOPRIVSNEEDED
// "<channel> :You're not channel operator"
std::string Replies::ERR_CHANOPRIVSNEEDED(const std::string &channel) {
  return (":jf.irc 482 * " + channel + " :You're not channel operator");
}

// 442     ERR_NOTONCHANNEL
// "<channel> :You're not on that channel"
std::string Replies::ERR_NOTONCHANNEL(const std::string &channel) {
  return (":jf.irc 442 * " + channel + " :You're not on that channel");
}

// >> :juan!aldana@localhost KICK #uva faraujo :sair
// >> :araujo!faaraujo@localhost PART #test1 :Leaving
std::string Replies::KICK_USER(const std::string &cmd, const std::string &nick,
                               const std::string &name,
                               const std::string &channel,
                               const std::string &target,
                               const std::string &reason) {
  std::string kickMessage = ":" + nick + "!" + name + "@localhost " + cmd +
                            " " + channel + " " + target + reason;
  return (kickMessage);
}
std::string Replies::PART_USER(const std::string &cmd, const std::string &nick,
                               const std::string &name,
                               const std::string &channel,
                               const std::string &reason) {
  std::string partMessage =
      ":" + nick + "!" + name + "@localhost " + cmd + " " + channel + reason;
  return (partMessage);
}

//  341    RPL_INVITING
// "<channel> <nick>"
// :ChanServ!ChanServ@example.com INVITE Attila #channel

std::string Replies::RPL_INVITING(const std::string &cmd,
                                  const std::string &nick,
                                  const std::string &channel) {
  return (":jf.irc 443 " + cmd + " " + nick + " " + channel);
}

// 441     ERR_USERNOTINCHANNEL
// "<nick> <channel> :They aren't on that channel"

std::string Replies::ERR_USERNOTINCHANNEL(const std::string &nick,
                                          const std::string &channel,
                                          const std::string &nickTarget) {
  std::string message = ":server 441 " + nick + " " + channel +
                        " " + nickTarget + " :They aren't on this channel\r\n";
  return (message);
}