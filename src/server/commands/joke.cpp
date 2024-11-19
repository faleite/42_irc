#include "../Server.hpp"

void Server::joke(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  std::string url = "http://official-joke-api.appspot.com/jokes/random";
  (void)cmd;
  static std::string punchLine;
  static std::string keyWord;
  static bool jokeRequested = false;

  Galois joke;
  if (param.size() == 0)
  {
    return;
  }
  if (param.size() == 1 && !param[0].empty() && param[0] == "faleiBot")
  {
    jokeRequested = true;
    std::string message = joke.getResponseJoke(url);
    std::string setUp = joke.getSetUp();
    keyWord = setUp.substr(0, setUp.find(" "));
    punchLine = joke.getPunchLine();
    client.getMessage("faleiBot : " + joke.getSetUp());
  }
  if (param.size() >= 2 && param[0] == "faleiBot" && !param[1].empty() &&
           jokeRequested)
  {
      client.getMessage("faleiBot : " + punchLine);
  }
}