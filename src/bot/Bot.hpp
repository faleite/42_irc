#ifndef BOT_HPP
#define BOT_HPP

#include "../client/Client.hpp"

// Rules.
// Command Parsing.
// Idea is to create a bot per channel.
// GENERAL - generalBot
// HELP - helpBot
// Random - randobot
// NEWS - reporterBot
// MOTIVATION - couchBot

class Bot : public Client {

public:
  // Canonical Form.
  Bot();
  Bot(int const &_botSoket, std::string const &name);
  ~Bot(void) { std::cout << "Bot Destroyed" << std::endl; };

  void processMessage(const std::string &message);
  void welcomeMessage();
};

#endif