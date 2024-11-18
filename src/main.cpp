#include "./server/Server.hpp"
#include <sstream>

bool stringToInt(const std::string &str, int &port);
void printPorts();

int main(int argc, char *argv[]) {

  int port;

  if (argc < 3) {
    std::cerr << "Wrong use\nCorrect usage : ./ircserv <port> <pass>"
              << std::endl;
    return (1);
  }
  if (!stringToInt(argv[1], port) || (port < 1024 || port > 65535)) {
    std::cerr << "Wrong Port, please try another one!" << std::endl;
    printPorts();
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

// Function to print the port ranges and their use cases.
void printPorts() {
    std::cout << "\n+-------------------------------------------------------------+\n"
              << "| Port Ranges and Their Use Cases:                            |\n"
              << "+-------------------------------------------------------------+\n"
              << "| 1. Well-Known Ports (0-1023):                               |\n"
              << "|   - Reserved for standard services such as HTTP, FTP, SSH.  |\n"
              << "+-------------------------------------------------------------+\n"
              << "| 2. Registered Ports (1024-49151):                           |\n"
              << "|   - Used by registered applications and services.           |\n"
              << "+-------------------------------------------------------------+\n"
              << "| 3. Dynamic or Private Ports (49152-65535):                  |\n"
              << "|   - Ephemeral ports for temporary use.                      |\n"
              << "+-------------------------------------------------------------+\n";
}

// Function to convert a string to an integer.
bool stringToInt(const std::string &str, int &port) {
  std::stringstream ss(str);
  ss >> port;
  if (ss.fail() || !ss.eof())
    return (false);
  return (true);
}

