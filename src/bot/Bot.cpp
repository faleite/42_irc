#include "Bot.hpp"

// Idea Bot.

// Created at the beggining of the code
// to provide some help, with info, funcitons, and etc.

// #_________________________ CONSTRUCTOR

Bot::Bot() { std::cout << "Defatult Boot Created" << std::endl; }

Bot::Bot(int const &botSocket, std::string const &name) : Client() {
  _clientSocket = botSocket;
  _name = name;
  _nickName = _name + "Bot";
  _isOperator = true;
}

void Bot::welcomeMessage() { sendMessage("Welcome, to the Channel"); }
void Bot::processMessage(const std::string &message) {
  if (message.find("?"))
    sendMessage("Well Well Well, you again with Another question");
}