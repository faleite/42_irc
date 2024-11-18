#include "HTTPClient.hpp"

class Galois : public HTTPClient {

private:
  std::string _setup;
  std::string _punchLine;

public:
  Galois();
  ~Galois();

  std::string getResponseMath(std::string const &_url);
  std::string getResponseJoke(std::string const &_url);

  //_______ Getter.
  std::string getSetUp();
  std::string getPunchLine();

};