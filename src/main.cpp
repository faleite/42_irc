#include "./server/Server.hpp"

// 	MAKE THE FUNC AUX PARSE

int	main(void)
{
	try
	{
		Server server("6667", "1234");
	}
	catch(const std::exception& e)
	{
		// CLOSE FILE DESCRIPTORS HERE
		std::cerr << e.what() << std::endl;
	}
	
	return (0);
}
