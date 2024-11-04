#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <string>

class HTTPClient {
public:
    HTTPClient();
    ~HTTPClient();
    std::string get(const std::string& url);
private:
    std::string extractHost(const std::string& url);
    std::string extractPath(const std::string& url);
};

#endif // HTTPCLIENT_HPP
