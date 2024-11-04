#include "Replies.hpp"

std::string  Replies::ERR_NEEDMOREPARAMS(const std::string &cmd)
{
  return (":jf.irc 461 * " + cmd + " :Not enough parameters");
}

std::string  Replies::ERR_UNKNOWNCOMMAND(const std::string &cmd)
{
  return (":jf.irc 421 * " + cmd + " :Unknown command");
}

std::string  Replies::ERR_PASSWDMISMATCH()
{
  return(":jf.irc 464 * :Password incorrect");
}

std::string  Replies::ERR_NOTREGISTERED()
{
  return(":jf.irc 451 * :You need to register before you can use that command"); // ":You have not registered"
}

std::string  Replies::ERR_ALREADYREGISTRED()
{
  return(":jf.irc 462 * :You may not reregisterd");
}

std::string  Replies::ERR_NICKNAMEINUSE(const std::string &nick)
{
  return(":jf.irc 433 * " + nick + " :Nickname is already in use");
}

