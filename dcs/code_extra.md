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