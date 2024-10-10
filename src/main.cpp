#include "Server.hpp"

int	main(void)
{
	Server server("54000");
	
	close(server.getSocketfd());

	// Enquanto escuta mostrar a menssagem, ecoar a menssagem
	char buffer[4096];
	while (true)
	{
		// Limpar o buffer
		memset(buffer, 0, 4096);
		// Esperar pela menssagem
		int bytesRecv = recv(server.getCliSocket(), buffer, 4096, 0);
		if (bytesRecv == - 1)
		{
			std::cerr << "There was a connection issue" << std::endl;
			break ;
		}
		if (bytesRecv == 0)
		{
			std::cout << "The client disconnected" << std::endl;
			break ;
		}
		// Mostrar menssagem
		std::cout << "Received: " << std::string(buffer, 0, bytesRecv);

		// Reenviar a menssagem
		send(server.getCliSocket(), buffer, bytesRecv + 1, 0); 
	}

	// Fechar o socket
	close(server.getCliSocket());

	return (0);
}
