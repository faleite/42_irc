#include "Server.hpp"

int Server::connectionRegistration(Client &client, std::string &message)
{
    std::string msg = message;
    if (msg.empty() || msg == ".\r\n") 
        return 1;

    msg.erase(remove(msg.begin(), msg.end(), '\r'), msg.end());
    std::vector<std::vector<std::string> > allTokens = this->tokenization(msg);

    for (size_t i = 0; i < allTokens.size(); ++i) 
    {
      std::vector<std::string> tokens = allTokens[i];
      if (tokens.empty())
        continue;

      std::string cmd = tokens[0];
      std::vector<std::string> params(tokens.begin() + 1, tokens.end());
      this->removeWhitespaceParams(params);

      for (size_t j = 0; j < cmd.length(); ++j) 
        cmd[j] = toupper((unsigned char)cmd[j]);
      
      this->registerNewUser(client, cmd, params);
    }
    if (!client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
    {
      std::cout << "Client has been disconnected :fd: " << client.getSocket() << std::endl;
      client.getMessage(Replies::ERR_PASSWDMISMATCH());
      this->cleanClient(client.getSocket());
      close(client.getSocket());
      return (1);
    }
    if (client.getAuthenticator() && !client.getNickName().empty() && !client.getName().empty())
    {
      client.setRegistered(true);
      this->sendWelcomeMessage(client);
      this->brodcastMessage(client.getName() + " Has Join to the server" );
    }
    return (0);
}

int checkInvalidCommand(const std::string &cmd)
{
  // See if exist others commands already
  if (cmd == JOIN || cmd == KICK || cmd == INVITE || cmd == LIST
      || cmd == MODE || cmd == PRIVMSG || cmd == PRIVMSG || cmd == SEND)
    return (1);
  else if (cmd != PASS && cmd != NICK 
            && cmd != USER && cmd != QUIT)
    return (2);
  return (0);
}

void Server::registerNewUser(Client &client, const std::string &cmd, const std::vector<std::string> &param)
{
  int checkCmd = checkInvalidCommand(cmd);
  if (checkCmd)
  {
    if (checkCmd == 1)
      client.getMessage(Replies::ERR_NOTREGISTERED());
    if (checkCmd == 2)
      client.getMessage(Replies::ERR_UNKNOWNCOMMAND(cmd));
    return ;
  }
  if (cmd == QUIT)
    this->quit(client, cmd, param);
  if (param.empty())
  {
    client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
    return ;
  }
  if (cmd == PASS)
  {
    if (!client.getAuthAttempted())
    {
      client.setAuthAttempted(true);
      if (param[0] == getPass())
        client.setAuthenticated(true);
      else
        client.setAuthenticated(false);
    }
  }
  else if (cmd == NICK)
  {
    	for (size_t i = 0; i < _clients.size(); ++i)
	    {
	    	if (param[0] == _clients[i].getNickName())
	    	{
	    		client.getMessage(Replies::ERR_NICKNAMEINUSE(param[0]));
        	return ;
	    	}
	    }
      client.setNickName(param[0]);
  }
  else if (cmd == USER)
  {
    if (param.size() < 4)
    {
      client.getMessage(Replies::ERR_NEEDMOREPARAMS(cmd));
      return ;
    }
    client.setName(param[0]);
  }
}

std::vector<std::vector<std::string> > Server::tokenization(const std::string &message) 
{
    std::vector<std::vector<std::string> > allTokens;
    std::istringstream iss(message);
    std::string line;

    while (getline(iss, line, '\n')) {
        std::vector<std::string> tokens;
        std::istringstream lineStream(line);
        std::string token;

        while (getline(lineStream, token, ' ')) {
            tokens.push_back(token);
        }
        allTokens.push_back(tokens);
    }
    return allTokens;
}

bool Server::removeWhitespaceParams(std::vector<std::string> &params) 
{
  bool vectorEmpty = true;
  for (size_t j = 0; j < params.size(); ++j) 
  {
    if (!params[j].empty() && params[j].find_first_not_of(" \t\n\v\f\r") != std::string::npos) 
    {
        vectorEmpty = false;
        break;
    }
  }
  if (vectorEmpty)
    params.clear();
  return vectorEmpty;
}