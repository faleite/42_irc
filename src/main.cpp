#include "./server/Server.hpp"
#include <sstream>

// 	MAKE THE FUNC AUX PARSE

bool stringToInt(const std::string &str, int &port) {
  std::stringstream ss(str);
  ss >> port;
  if (ss.fail() || !ss.eof())
    return (false);
  return (true);
}

int main(int argc, char *argv[]) {

  int port;

  if (argc < 3) {
    std::cerr << "Wrong use\nCorrect usage : ./ircerver <port> <pass>"
              << std::endl;
    return (1);
  }
  if (!stringToInt(argv[1], port)) {
    std::cerr << "Wrong Port please try another one" << std::endl;
    return (1);
  }
  try {
    Server server(port, argv[2]);
    server.registerSignalHandler();
    server.initServer();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return (0);
}