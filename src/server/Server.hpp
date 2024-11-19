#ifndef SERVER_HPP
#define SERVER_HPP

#include "../client/Client.hpp"
#include "../client/HTTPClient.hpp"
#include "../replies/Replies.hpp"
#include "../client/Galois.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#define JOIN "JOIN"
#define KICK "KICK"
#define INVITE "INVITE"
#define TOPIC "TOPIC"
#define MODE "MODE"
#define MSG "MSG"
#define SEND "SEND"
#define LIST "LIST"
#define PRIVMSG "PRIVMSG"
#define PASS "PASS"
#define NICK "NICK"
#define USER "USER"
#define QUIT "QUIT"
#define PART "PART"
#define JOKE "JOKE"
#define MATH "MATH"
#define CAP "CAP"
#define WHO "WHO"

#define BUFFER_SIZE 1024

class Channel;
class Server
{
private:
	int _sockfd;
	int _port;
	std::string _pass;
	std::vector<Client *> _clients;
	std::vector<struct pollfd> _pfds;
	static Server *instance;
	bool _signal;
	std::map<std::string, Channel> _channels;
public:
	Server();
	Server(const Server &copyObj);
	Server &operator=(const Server &assignCopy);
	Server(int const &port, std::string pass);
	~Server();

	// ________________________ CONNECTION HANDLER
	void createSocket();
	void acceptClient();
	void initServer();
	void cleanClient(int fd);
	void closeFds();
	void stop();
	void registerSignalHandler();
	static void closeSignal(int sig);

	// ________________________ MSG HANDLER.
	void brodcastMessage(std::string const &messge);
	void sendWelcomeMessage(Client newClient);
	std::string getMessage(int fd);
	void handleMessage(int fd);

	// ________________________ COMMANDS HANDLER.
	void pass(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void nick(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void user(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void quit(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	// Private Messagge Logic.
	void privmsg(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	//__________________________ Channel Logic.
	void join(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void list(Client &client, const std::string &cmd, const std::vector<std::string> &param);

	//_________________________CHANNEL MODES
	void mode(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void topic(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void invite(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void kick(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void part(Client &client, const std::string &cmd, const std::vector<std::string> &param);

	typedef void (Server::*CommandFunc)(Client &, const std::string &, const std::vector<std::string> &);
	std::map<std::string, CommandFunc> commandMap;

	//_________________________ Bot.
	void math(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	void joke(Client &client, const std::string &cmd, const std::vector<std::string> &param);

	// ________________________ CHANNEL MESSAGE.
	void createChannel(std::string const &name);
	bool findChannel(std::string const &channelName);

	// ________________________ AUTHENTICATION.
	std::string const &getPass() const;
	int connectionRegistration(Client &client, std::string &message);
	void registerNewUser(Client &client, const std::string &cmd, const std::vector<std::string> &param);
	int commands(Client &client, std::string &message);
};

bool stringToInt(const std::string &str, int &port);
#endif // SERVER_HPP