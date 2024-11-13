# CODE EXTRA


## SEND MESSAGE (MSG)
```cpp
//________________________________SEND MESSAGE IN THE CHANNEL

void Server::msg(Client &client, const std::string &cmd,
                 const std::vector<std::string> &param)
{
  (void)cmd;
  std::string mess = client.getName() + " : ";
  std::ostringstream oss;
  for (size_t i = 1; i < param.size(); ++i)
  {
    if (i != 0)
      oss << " ";
    oss << param[i];
  }
  mess += oss.str();
  if (findChannel(param[0]))
  {
    _channels[param[0]].isOnChannel(client.getNickName())
        ? _channels[param[0]].broadcastMessage(mess, &client)
        : client.getMessage("You are not part of this channel");
  }
  else
    client.getMessage("Channel not found");
}
```

## JOIN TO A CHANNEL
```cpp
void Server::join(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  (void)cmd;
  try
  {
    std::map<std::string, Channel>::iterator it = _channels.find(param[0]);
    if (it != _channels.end())
    {
      std::cout << ":::::::::::::::::::: Chanel Found :::::::" << param[0]
                << std::endl;
      (param.size() > 1) ? _channels[param[0]].joinChannel(&client, param[1])
                         : _channels[param[0]].joinChannel(&client, "");
    }
    else
    {
      std::cout << ":::::::::::::::::::: Creating Channel :::::::" << param[0]
                << std::endl;
      createChannel(param[0]);
      it = _channels.find(param[0]);
      it->second.joinChannel(&client, "");
    }
    client.getMessage(
        Replies::RPL_WELCOME(client.getNickName(), client.getName(), param[0]));
  }
  catch (std::exception &e)
  {
    std::cout << "Error :::" << e.what() << std::endl;
    client.getMessage(e.what());
  }
}
```

## Handle Message on Client Side
```cpp
std::string Client::receiveMessage() {
  char buffer[1024];
  int bytesReceived = recv(_clientSocket, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived == -1) {
    std::cerr << "Client Disconnected" << std::endl;
    return ("");
  } else if (bytesReceived == 0) {
    std::cerr << "Client disconnected" << std::endl;
    return "";
  }
  std::cout << "Message received " << buffer << std::endl;
  return (buffer);
}
```

## Delete Client from Server
```cpp
void Server::cleanClient(int fd)
{
	try
	{
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end();)
		{
			if (it->getClientSoket() == fd)
			{
				it = _clients.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error("Error clean Client");
	}
}
```