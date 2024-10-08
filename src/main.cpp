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

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <iostream>
// #include <cstring>

// int main() {
//     int server_fd, client_fd;
//     struct sockaddr_in server_addr, client_addr;
//     socklen_t client_addr_len = sizeof(client_addr);
//     char buffer[1024];
//     int port = 6667; // Default IRC port

//     // Create the server socket
//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd < 0) {
//         perror("socket");
//         return 1;
//     }

//     // Set up the server address struct
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET; // Set the address family to IPv4
//     server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
//     server_addr.sin_port = htons(port); // Convert the port to network byte order

//     // Bind the socket to the address and port
//     if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
//         perror("bind");
//         close(server_fd);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(server_fd, 5) < 0) {
//         perror("listen");
//         close(server_fd);
//         return 1;
//     }

//     std::cout << "Server listening on port " << port << std::endl;

//     // Accept a connection
//     client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
//     if (client_fd < 0) {
//         perror("accept");
//         close(server_fd);
//         return 1;
//     }

//     std::cout << "Client connected" << std::endl;

//     // Read data from the client
//     ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
//     if (bytes_read < 0) {
//         perror("read");
//         close(client_fd);
//         close(server_fd);
//         return 1;
//     }

//     buffer[bytes_read] = '\0'; // Null-terminate the buffer
//     std::cout << "Received message: " << buffer << std::endl;

//     // Close the client and server sockets
//     close(client_fd);
//     close(server_fd);

//     return 0;
// }