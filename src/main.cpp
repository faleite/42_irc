#include "./server/Server.hpp"

Server *server = NULL;

// 	MAKE THE FUNC AUX PARSE

	void closeSignal(int sig) {
		if(!server)
			std::cout << "There is no server to shutdown\n";
		if (sig == SIGINT)
		{
			server->clientExit();
			server->stop();
		}
	}


int	main(void)
{
	// Server *server = NULL;
	
	server = new Server("6667", "1234");
	signal(SIGINT, closeSignal);
	server->initServer();
	delete (server);
	return (0);
}
