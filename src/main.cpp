#include "./server/Server.hpp"
#include <sstream>

Server *server = NULL;

// 	MAKE THE FUNC AUX PARSE

bool stringToInt(const std::string &str, int &port) {
  std::stringstream ss(str);
  ss >> port;
  if (ss.fail() || !ss.eof())
    return (false);
  return (true);
}
void closeSignal(int sig) {
  if (!server)
    std::cout << "There is no server to shutdown\n";
  if (sig == SIGINT) {
    server->clientExit();
    server->stop();
  }
}

int main(int argc, char *argv[]) {
  // Server *server = NULL;
  int port;

  if (argc < 3) {
    std::cerr << "Incorrect use \nCorrect use : ./irserver <port> <password>"
              << std::endl;
    return (1);
  }
  if (stringToInt(argv[1], port))
    server = new Server(port, argv[2]);
  else {
    std::cerr << "Wrong Port please try another one" << std::endl;
    return (1);
  }
  signal(SIGINT, closeSignal);
  server->initServer();
  delete server;
  return (0);
}
