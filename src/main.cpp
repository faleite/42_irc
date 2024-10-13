#include "Server.hpp"

int	main(void)
{
	Server server("6667");
		
	close(server.getSocketfd());
	close(server.getCliSocket());

	return (0);
}
