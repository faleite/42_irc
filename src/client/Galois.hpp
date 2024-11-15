#include "HTTPClient.hpp"

class Galois : public HTTPClient {

private:
  std::string _setup;
  std::string _punchLine;

public:
  Galois();
  ~Galois();

  std::string getResponseMath(std::string const &_response);
  std::string getResponseJoke(std::string const &_response);

  //_______ Getter.
  std::string getSetUp();
  std::string getPunchLine();
  
};