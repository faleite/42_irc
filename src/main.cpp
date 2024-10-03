#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

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
	if (bind(listening, &hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/port";
		return (-2);
	}
	// https://youtu.be/cNdlrbZSkyQ?si=G8piqyqn53PR9hSi
	
	// Marcar o socket para ouvir
	// Aceitar a chamada
	// Fechar a escuta do socket
	// Enquanto escuta mostrar a menssagem, ecoar a menssagem
	// Fechar o socket
	
}