# CODE EXTRA

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