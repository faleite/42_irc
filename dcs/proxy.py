import socket
import select

# Configuração do Proxy
LISTEN_HOST = '127.0.0.1'  # Host onde o proxy vai escutar
LISTEN_PORT = 6666         # Porta do proxy
IRC_SERVER = '127.0.0.1'   # Endereço do servidor IRC que será depurado
IRC_PORT = 6667            # Porta do servidor IRC

def handle_client(client_socket, irc_socket):
    sockets = [client_socket, irc_socket]
    
    while True:
        # Usar select para monitorar os sockets para leitura
        read_sockets, _, _ = select.select(sockets, [], [])
        
        for sock in read_sockets:
            if sock == client_socket:
                # Receber dados do cliente IRC
                data = client_socket.recv(4096)
                if not data:
                    print("Conexão encerrada pelo cliente")
                    exit() # return
                print(f"[CLIENTE -> SERVIDOR] {data.decode('utf-8')}")
                irc_socket.send(data)
            
            elif sock == irc_socket:
                # Receber dados do servidor IRC
                data = irc_socket.recv(4096)
                if not data:
                    print("Conexão encerrada pelo servidor")
                    return
                
                print(f"[SERVIDOR -> CLIENTE] {data.decode('utf-8')}")
                client_socket.send(data)

def start_proxy():
    # Criar o socket do proxy
    proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    proxy_socket.bind((LISTEN_HOST, LISTEN_PORT))
    proxy_socket.listen(5)
    print(f"Proxy escutando em {LISTEN_HOST}:{LISTEN_PORT}")
    
    while True:
        # Aceitar conexões do cliente IRC
        client_socket, addr = proxy_socket.accept()
        print(f"Conexão aceita de {addr}")
        
        # Conectar ao servidor IRC real
        irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        irc_socket.connect((IRC_SERVER, IRC_PORT))
        
        # Manter a comunicação entre cliente e servidor
        handle_client(client_socket, irc_socket)

if __name__ == "__main__":
    start_proxy()
