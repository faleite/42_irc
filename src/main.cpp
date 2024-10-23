#include "./server/Server.hpp"

int	main(void)
{
	try
	{
		Server server("6667", "1234");
		server.registerSignalHandler();
		server.initServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
