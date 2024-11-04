#include "HTTPClient.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

HTTPClient::HTTPClient() {}

HTTPClient::~HTTPClient() {}

std::string HTTPClient::extractHost(const std::string& url) {
    std::string protocol("http://");
    size_t protocolEnd = url.find(protocol);
    size_t hostStart = protocolEnd + protocol.size();
    size_t hostEnd = url.find('/', hostStart);
    return url.substr(hostStart, hostEnd - hostStart);
}

std::string HTTPClient::extractPath(const std::string& url) {
    size_t pathStart = url.find('/', url.find("http://") + strlen("http://"));
    return url.substr(pathStart);
}

std::string HTTPClient::get(const std::string& url) {
    std::string host = extractHost(url);
    std::string path = extractPath(url);

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host.c_str(), "80", &hints, &res);
    if (err != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
        return "";
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        std::cerr << ("socket") << std::endl;
        return "";
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << ("connect") << std::endl;
        close(sockfd);
        return "";
    }

    std::ostringstream request;
    request << "GET " << path << " HTTP/1.0\r\n";
    request << "Host: " << host << "\r\n";
    request << "Connection: close\r\n\r\n";

    std::string requestStr = request.str();
    if (send(sockfd, requestStr.c_str(), requestStr.size(), 0) == -1) {
        std::cerr << ("send") << std::endl;
        close(sockfd);
        return "";
    }

    char buffer[4096];
    std::string response;
    int bytesRead;
    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytesRead);
    }

    close(sockfd);
    return response;
}
