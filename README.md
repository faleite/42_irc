```
 ██╗  ██████╗    ██████╗
 ██║  ██╔══██╗  ██╔════╝
 ██║  ██████╔╝  ██║
 ██║  ██╔══██╗  ██║
 ██║  ██║  ██║  ╚██████╗
 ╚═╝  ╚═╝  ╚═╝   ╚═════╝
```

*Create your own IRC server in C++, fully compatible with an official client.*

 Skills | Grade |
:------:|:-----:|
[**Network & system administration**] [**Object-oriented programming**] [**Unix**] [**Rigor**] | *Subscribed* |
<!-- **:white_check_mark: 100%** -->

<!-- * **[Introduction](#introduction)**
  * **[General rules](#general-rules)**
  * **[design requirements](#alguns-requisitos-de-design)**
  * **[Read me](#read-me)**
  * **[Orthodox Canonical Form](#orthodox-canonical-form)**
* **[Module-specific rules](#module-specific-rules)** -->
* **[Mandatory part](#mandatory-part)**
* **[Study resources](#study-resources)**
<!-- * ### [Usage]() -->
<!-- * ### [Workflow](#workflow-1) -->
<!-- * ### [Tools](#tools-1) -->

<details>
  <summary><b>See</b></summary>

- <cstring> (c++) vs <string.h> (c)
- **Boost libraries** (are forbidden).

</details>

## Mandatory part
<details>
  <summary><b>External functs.</b></summary>

*Essas funções são todas utilizadas para manipulação de sockets e operações\
de entrada/saída no ambiente Unix, com compatibilidade com C++ 98.*

Funcoes externas | Header | Brief
------- | ------ | -----
**socket**        | `<sys/socket.h>` | Cria um ponto de comunicação, usado para comunicação de rede.        
**close**         | `<unistd.h>`     | Fecha um descritor de arquivo, incluindo sockets.                    
**setsockopt**    | `<sys/socket.h>` | Configura opções de um socket, como tempo de espera ou buffer.       
**getsockname**   | `<sys/socket.h>` | Recupera o endereço associado a um socket.                          
**getprotobyname**| `<netdb.h>`      | Recupera informações sobre protocolos a partir de um nome.           
**gethostbyname** | `<netdb.h>`      | Recupera o endereço IP de um host dado o nome de domínio.            
**getaddrinfo**   | `<netdb.h>`      | Resolve hostnames para endereços IP, substituto moderno de `gethostbyname`.
**freeaddrinfo**  | `<netdb.h>`      | Libera a estrutura alocada pela função `getaddrinfo`.                
**bind**          | `<sys/socket.h>` | Associa um endereço ao socket.                                       
**connect**       | `<sys/socket.h>` | Conecta um socket a um endereço remoto.                              
**listen**        | `<sys/socket.h>` | Marca um socket para aceitar conexões de entrada.                    
**accept**        | `<sys/socket.h>` | Aceita uma conexão de entrada em um socket.                          
**htons**         | `<arpa/inet.h>`  | Converte números de porta de host para rede em ordem de bytes.       
**htonl**         | `<arpa/inet.h>`  | Converte um inteiro de 32 bits de host para rede em ordem de bytes.  
**ntohs**         | `<arpa/inet.h>`  | Converte números de porta de rede para host em ordem de bytes.       
**ntohl**         | `<arpa/inet.h>`  | Converte um inteiro de 32 bits de rede para host em ordem de bytes.  
**inet_addr**     | `<arpa/inet.h>`  | Converte um endereço IP no formato string para um valor numérico.    
**inet_ntoa**     | `<arpa/inet.h>`  | Converte um valor numérico de IP para uma string legível.            
**send**          | `<sys/socket.h>` | Envia dados através de um socket.                                    
**recv**          | `<sys/socket.h>` | Recebe dados de um socket.                                           
**signal**        | `<signal.h>`     | Configura manipuladores de sinais de sistema.                        
**sigaction**     | `<signal.h>`     | Define ações personalizadas para sinais.                             
**lseek**         | `<unistd.h>`     | Move o ponteiro de leitura/escrita de um arquivo para uma nova posição.
**fstat**         | `<sys/stat.h>`   | Obtém informações sobre um arquivo aberto.                           
**fcntl**         | `<fcntl.h>`      | Manipula descritores de arquivos, como bloquear ou configurar flags. 
**poll**          | `<poll.h>`       | Monitora múltiplos descritores de arquivo para verificar eventos.

</details>


## Study resources
Resource | Source
---------|:-----:
[CPP Tutorial (C++98)](https://cplusplus.com/doc/oldtutorial) | `Website`
[CPP Reference](https://cplusplus.com/) | `Website`
[Wiki 42cpp](https://github.com/faleite/CPP_42_School/wiki) | `Wiki`