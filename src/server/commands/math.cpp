#include "../Server.hpp"

void Server::math(Client &client, const std::string &cmd,
                  const std::vector<std::string> &param)
{
  std::string url = "http://numbersapi.com/random/math";
  (void)cmd;
  Galois math;
  if (param.size() == 0)
    return;
  if (param.size() == 1 && !param[0].empty() && param[0] == "juanBot")
  {
    client.getMessage("juanBot : Do you want to know a math fact? 🤓");
  }
  else if (param.size() == 2 && !param[0].empty() && !param[1].empty())
  {
    std::string message = math.getResponseMath(url);
    if (param[1] == "please" && param[0] == "juanBot")
    {
      client.getMessage("🤓 juanBot : ⚛" + message);
    }
    else
    {
      client.getMessage("You did not said the magic word. 🤺");
    }
  }
}