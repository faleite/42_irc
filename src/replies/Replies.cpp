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

std::string Replies::ERR_NOSUCHCHANNEL(const std::string &nick, const std::string &channel) {
  return (":jf.irc 403 " + nick + " " + channel + " :No such channel");
}


std::string Replies::LEAVE_CHANNEL(const std::string &nick,
                                 const std::string &user,
                                 const std::string &channel) {
 return std::string(":" + nick + "!" + user + "@localhoost" + " PART " +
         channel);
}