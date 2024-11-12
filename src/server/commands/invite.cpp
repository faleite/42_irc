#include "../Server.hpp"

//___________Invite to a channel
// Command : IVITE <#channel> <user>
// invite username #channelname

void Server::invite(Client &client, const std::string &cmd,
                    const std::vector<std::string> &param)
{
  (void)cmd;

  if (param.size() < 2 || param[1].empty() || !client.getIsOperator())
    return;
  if (findChannel(param[1]))
  {
    for (std::vector<Client *>::iterator iter = _clients.begin();
         iter != _clients.end(); iter++)
    {
      if ((*iter)->getNickName() == param[0])
      {
        _channels[param[1]].invite(client.getNickName(), (*iter));
        return;
      }
    }
    client.getMessage("User Not Found");
  }
}