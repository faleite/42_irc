### 1. Planejamento do Projeto
- **Objetivo:** Desenvolver um servidor IRC que suporte múltiplos clientes, canais, mensagens privadas e comandos básicos do protocolo IRC.
- **Requisitos:** 
  - Suporte a múltiplos clientes simultâneos.
  - Implementação de comandos IRC básicos (NICK, USER, JOIN, PART, PRIVMSG, QUIT).
  - Gerenciamento de canais.
  - Tratamento de mensagens privadas.
  - Conformidade com a versão C++98.

### 2. Estrutura do Projeto
- **Diretórios:**
  - `src`: Código-fonte do servidor.
  - `include/`: Arquivos de cabeçalho.
  - `build/`: Arquivos de compilação.
  - `tests/`: Testes unitários.
  - `docs/`: Documentação.

### 3. Arquivos Principais
- `main.cpp`: Ponto de entrada do servidor.
- `src/Server.cpp`: Implementação da classe `Server`.
- `src/Client.cpp`: Implementação da classe `Client`.
- `src/Channel.cpp`: Implementação da classe `Channel`.
- `include/Server.h`: Declaração da classe `Server`.
- `include/Client.h`: Declaração da classe `Client`.
- `include/Channel.h`: Declaração da classe `Channel`.

### 4. Classes Principais
- **Server:**
  - Responsável por aceitar conexões de clientes.
  - Gerenciar a comunicação entre clientes e canais.
  - Manter a lista de clientes conectados e canais ativos.
- **Client:**
  - Representa um cliente conectado ao servidor.
  - Gerencia o estado do cliente (nickname, canais, etc.).
  - Processa comandos recebidos do cliente.
- **Channel:**
  - Representa um canal de chat.
  - Gerencia a lista de clientes no canal.
  - Envia mensagens para todos os clientes no canal.

### 5. Funcionalidades Básicas
- **Conexão e Desconexão:**
  - Aceitar novas conexões de clientes.
  - Remover clientes desconectados.
- **Autenticação:**
  - Processar comandos NICK e USER para autenticar clientes.
- **Gerenciamento de Canais:**
  - Criar e deletar canais.
  - Adicionar e remover clientes de canais.
- **Mensagens:**
  - Enviar mensagens privadas (PRIVMSG).
  - Enviar mensagens para canais.
- **Comandos:**
  - Implementar comandos básicos do protocolo IRC.

### 6. Ferramentas e Bibliotecas
- **Compilador:** g++ com suporte a C++98.
- **Bibliotecas:** 
  - `boost::asio` para operações de rede (opcional, se permitido pelo C++98).
  - Alternativamente, usar sockets POSIX diretamente.

### 7. Compilação e Execução
- **Makefile:** Criar um Makefile para compilar o projeto.
- **Execução:** Instruções para executar o servidor e conectar clientes.

### 8. Testes
- **Testes Unitários:** Escrever testes para cada classe e funcionalidade.
- **Testes de Integração:** Testar a comunicação entre múltiplos clientes e o servidor.

### 9. Documentação
- **README.md:** Instruções de uso e descrição do projeto.
- **Documentação do Código:** Comentários e documentação das classes e métodos.

### 10. Controle de Versão
- **Git:** Utilizar Git para versionamento do código.
- **Repositório:** Configurar um repositório no GitHub ou similar.
