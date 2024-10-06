#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>

int	main(void)
{
	// Criar um socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cerr << "Can't create a socket!";
		return (-1);
	}

	// Vincular o socket a um IP / port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	if (bind(listening, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/port";
		return (-2);
	}
	
	// Marcar o socket para ouvir
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!";
		return (-3);
	}

	// Aceitar a chamada
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	int clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
	if (clientSocket == -1)
	{
		std::cerr << "Problem with client connecting!";
		return (-4);
	}

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr *)&client, clientSize, 
								host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result)
		std::cout << host << " connected on " << service << std::endl;
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
	
	// Fechar a escuta do socket
	close(listening);

	// Enquanto escuta mostrar a menssagem, ecoar a menssagem
	char buffer[4096];
	while (true)
	{
		// Limpar o buffer
		memset(buffer, 0, 4096);
		// Esperar pela menssagem
		int bytesRecv = recv(clientSocket, buffer, 4096, 0);
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
		std::cout << "Received: " << std::string(buffer, 0, bytesRecv) << std::endl;

		// Reenviar a menssagem
		send(clientSocket, buffer, bytesRecv + 1, 0); 
	}

	// Fechar o socket
	close(clientSocket);

	return (0);
}