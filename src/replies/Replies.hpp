#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "../server/Server.hpp"

class Replies
{
	public:
		static std::string  ERR_PASSWDMISMATCH();
		static std::string  ERR_NOTREGISTERED();
		static std::string  ERR_ALREADYREGISTRED();
		static std::string  ERR_NEEDMOREPARAMS(const std::string &cmd);
		static std::string  ERR_UNKNOWNCOMMAND(const std::string &cmd);
		static std::string  ERR_NICKNAMEINUSE(const std::string &nick);
};

#endif // REPLIES_HPP