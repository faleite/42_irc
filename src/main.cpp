#include "Server.hpp"

int	main(void)
{
	Server server("54000");
		
	close(server.getSocketfd());
	close(server.getCliSocket());

	return (0);
}
