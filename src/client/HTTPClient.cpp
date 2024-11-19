#include "HTTPClient.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

//____________________ Constructor and destructor.

HTTPClient::HTTPClient(): path(""), host(""), protocol("http://"){}
HTTPClient::~HTTPClient() {}

//____________________ Get the host part.
void HTTPClient::extractHost(const std::string &_url) {
  size_t protocolEnd = _url.find(protocol);
  size_t hostStart = protocolEnd + protocol.size();
  size_t hostEnd = _url.find('/', hostStart);
  host = _url.substr(hostStart, hostEnd - hostStart);
}

void  HTTPClient::extractPath(const std::string &_url) {

  size_t pathStart = _url.find('/', _url.find(protocol) + (protocol).size());
  path = _url.substr(pathStart);
}

std::string HTTPClient::get(const std::string &_url) {
  extractHost(_url);
  extractPath(_url);

  //__________________ set uf the Address info.
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  //________________Get Address inform for the host
  int err = getaddrinfo(host.c_str(), "80", &hints, &res);
  if (err != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
    return "";
  }

  //_____________Create A socket.
  int sockFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockFd == -1) {
    std::cerr << "socket" << std::endl;
    return "";
  }

  //____________ Connect to the server.
  if (connect(sockFd, res->ai_addr, res->ai_addrlen) == -1) {
    std::cerr << "connect" << std::endl;
    close(sockFd);
    return "";
  }

  //_____________ Construct the HTTP GET request
  std::ostringstream request;
  request << "GET " << path << " HTTP/1.1\r\n";
  request << "Host: " << host << "\r\n";
  request << "Connection: close\r\n\r\n";

  // Send the HTTP GET request
  std::string requestStr = request.str();
  if (send(sockFd, requestStr.c_str(), requestStr.size(), 0) == -1) {
    std::cerr << "send" << std::endl;
    close(sockFd);
    return "";
  }

  //_____________ Receive the Response.
  char buffer[4096];
  std::string response;
  int bytesRead;
  while ((bytesRead = recv(sockFd, buffer, sizeof(buffer), 0)) > 0) {
    response.append(buffer, bytesRead);
  }

  //_____________ Close
  close(sockFd);
  freeaddrinfo(res);
  return response;
}