#include "Galois.hpp"
#include <sstream>
#include <string>

Galois::Galois() : HTTPClient(), _setup(""), _punchLine("_") {}

Galois::~Galois() {}

std::string Galois::getResponseMath(const std::string &_url) {
  std::string response;
  std::stringstream ss(get(_url));
  std::string lastLine;

  while (std::getline(ss, response))
    lastLine = response;
  return response;
}

std::string Galois::getResponseJoke(const std::string &_url) {
  const std::string jokeSet = "\"setup\":\"";
  const std::string punchSet = "\"punchline\":\"";

  std::string response;
  std::stringstream ss(get(_url));
  std::string lastLine;

  while (std::getline(ss, response))
    lastLine = response;

  size_t jokeStart = lastLine.find(jokeSet);
  if (jokeStart != std::string::npos) {
    jokeStart += jokeSet.size();
    size_t jokeEnd = lastLine.find("\"", jokeStart);
    if (jokeEnd != std::string::npos) {
      _setup = lastLine.substr(jokeStart, jokeEnd - jokeStart);
    }
  }

  size_t punchStart = lastLine.find(punchSet);
  if (punchStart != std::string::npos) {
    punchStart += punchSet.size();
    size_t punchEnd = lastLine.find("\"", punchStart);
    if (punchEnd != std::string::npos) {
      _punchLine = lastLine.substr(punchStart, punchEnd - punchStart);
    }
  }

  return (lastLine);
}

std::string Galois::getSetUp() { return _setup; }
std::string Galois::getPunchLine() { return _punchLine; }