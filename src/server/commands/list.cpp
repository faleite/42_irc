#include "../Server.hpp"

// LIST Command Overview

// The LIST command is used to list channels and their topics. Here’s a breakdown of the associated numeric replies and what they signify:
// Numeric Replies for LIST Command
// ERR_NOSUCHSERVER (402)

//     Format: :<server> 402 <client> <server> :No such server

//     Description: Indicates that the specified server does not exist.

// RPL_LISTSTART (321)

//     Format: :<server> 321 <client> Channel :Users Name

//     Description: Marks the start of the list of channels.

// RPL_LIST (322)

//     Format: :<server> 322 <client> <channel> <number of visible users> :<topic>

//     Description: Provides the details of each channel.

//     Example: :irc.example.com 322 client #general 42 :General discussion channel

// RPL_LISTEND (323)

//     Format: :<server> 323 <client> :End of /LIST

//     Description: Marks the end of the list of channels.
void Server::list(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  (void)cmd;
  (void)param;
  std::string response = "Active Channels:\n";
  response += "| Channel Name | Users Count | Topic/Description |\n";
  response += "|--------------------|-------------|--------------------------|"
              "\n";
  for (std::map<std::string, Channel>::iterator it = _channels.begin();
       it != _channels.end(); ++it)
  {
    std::string channelName = it->first;
    int userCount = it->second.getUsers();
    std::string topic = it->second.getRestrictedTopic() ? "Restricted Topic"
                                                      : it->second.getTopic();

    std::stringstream row;
    row << "| " << std::left << std::setw(19) << channelName << "| "
        << std::left << std::setw(12) << userCount << "| " << std::left
        << std::setw(24) << topic << "|\n";
    response += row.str();
  }
  client.getMessage(response);
}