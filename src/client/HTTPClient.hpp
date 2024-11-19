#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>

class HTTPClient {
private:
  std::string path;
  std::string host;
  std::string protocol;
  void extractHost(std::string const &_url);
  void extractPath(std::string const &_url);

public:
  HTTPClient();
  ~HTTPClient();
  std::string get(std::string const &_url);
};

#endif
