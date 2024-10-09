#include "Server.hpp"

// int	main(void)
// {
// 	Server server("54000");
// 	// Aceitar a chamada
// 	sockaddr_in client;
// 	socklen_t clientSize = sizeof(client);
// 	char host[NI_MAXHOST];
// 	char service[NI_MAXSERV];

// 	int clientSocket = accept(server.getSocketfd(), (sockaddr *)&client, &clientSize);
// 	if (clientSocket == -1)
// 	{
// 		std::cerr << "Problem with client connecting!" << std::endl;
// 		return (-4);
// 	}

// 	memset(host, 0, NI_MAXHOST);
// 	memset(service, 0, NI_MAXSERV);

// 	int result = getnameinfo((sockaddr *)&client, clientSize, 
// 								host, NI_MAXHOST, service, NI_MAXSERV, 0);
// 	if (result)
// 		std::cout << host << " connected on " << service << std::endl;
// 	else
// 	{
// 		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
// 		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
// 	}
	
// 	// Fechar a escuta do socket
// 	close(server.getSocketfd());

// 	// Enquanto escuta mostrar a menssagem, ecoar a menssagem
// 	char buffer[4096];
// 	while (true)
// 	{
// 		// Limpar o buffer
// 		memset(buffer, 0, 4096);
// 		// Esperar pela menssagem
// 		int bytesRecv = recv(clientSocket, buffer, 4096, 0);
// 		if (bytesRecv == - 1)
// 		{
// 			std::cerr << "There was a connection issue" << std::endl;
// 			break ;
// 		}
// 		if (bytesRecv == 0)
// 		{
// 			std::cout << "The client disconnected" << std::endl;
// 			break ;
// 		}
// 		// Mostrar menssagem
// 		std::cout << "Received: " << std::string(buffer, 0, bytesRecv) << std::endl;

// 		// Reenviar a menssagem
// 		send(clientSocket, buffer, bytesRecv + 1, 0); 
// 	}

// 	// Fechar o socket
// 	close(clientSocket);

// 	return (0);
// }

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <iostream>
// #include <cstring>

int main() {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

	Server server("6667");

    // Accept a connection
    client_fd = accept(server.getSocketfd(), (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        herror("accept");
        close(server.getSocketfd());
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    // Read data from the client
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        herror("read");
        close(client_fd);
        close(server.getSocketfd());
        return 1;
    }

    buffer[bytes_read] = '\0'; // Null-terminate the buffer
    std::cout << "Received message: " << buffer << std::endl;

    // Close the client and server sockets
    close(client_fd);
    close(server.getSocketfd());

    return 0;
}