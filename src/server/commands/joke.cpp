#include "../Server.hpp"

// #define URLR url1 = "http://numbersapi.com/random/math" // Test URL
// #define URLC url2 =
// "http://universities.hipolabs.com/search?country=colombia"

// void Server::joke(Client &client, const std::string &cmd,
//                   const std::vector<std::string> &param) {
//   std::string url = "http://official-joke-api.appspot.com/jokes/random";
//   (void)cmd;
//   HTTPClient joke;
//   //
//   joke.getResponseJoke("http://official-joke-api.appspot.com/jokes/random");

//   if (param.size() == 1) {
//     std::string response = joke.getResponseJoke(url);
//     //   std::string response = client.get(url);
//     if (response.empty()) {
//       std::cout << "Failed to retrieve the response." << std::endl;
//     } else {
//       std::cout << "Response received: " << std::endl;
//       std::cout << response << std::endl; // Display the raw response
//     }
//     client.getMessage("FaleiteBot : ");
//   } else {
//     if (param[0] == "more") {
//       client.getMessage("FaleiteBot : ");
//     }
//   }
// }

void Server::joke(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param) {
  std::string url = "http://official-joke-api.appspot.com/jokes/random";
  (void)cmd;
  static std::string punchLine;
  Galois joke;
  if (param.size() == 0) {
    std::string message = joke.getResponseJoke(url);
    std::string setUp = joke.getSetUp();
    punchLine = joke.getPunchLine();
    client.getMessage(joke.getSetUp());
  }
  else
    client.getMessage("faleite : " + punchLine);
}