# Handle Messages

## Register user

### HexChat
- Para registrar um usuário no HexChat ele envia:
  - **<<** `CAP` LS 302
  - **<<** `PASS` 1234
  - **<<** `NICK` fabricio
  - **<<** `USER` faleite 0 * :realname
    - `faleite` é o nome do usuário
    - `0` é o modo do usuário
    - `*` é o host do usuário
    - `:realname` é o nome real do usuário

### PASS
- [ ] a `PASS` tem ser o primeiro ou segundo comando enviado pelo cliente.
  - [ ] se setar primeiro o `user` e `nick`, envia `pass` incorreta e fecha a conexão.
- [ ] Depois de enviar a `PASS`, `NICK` e `USER` vai a menssagem de erro da `pass`\
 ou boas vindas.
- [ ] `PASS` incorrecta, o servidor deve enviar uma mensagem de erro e fechar a\
 conexão.
- [ ] Após conectado nao pode enviar `PASS` e `USER` novamente.
  - [ ] envie msg: :You may not reregister

### NICK
- [ ] `NICK` pode ser enviado a qualquer momento.

### USER
[*RFC protocol*](https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3)\
Comando: `USER`\
Parâmetros: `<username> <mode> <unused> <realname>`\
Exemplo: `USER guest 0 * :Ronnie Reagan`
- [ ] `USER` deve ter 4 parâmetros. O quarto parâmetro pode ter espaços.
- [ ] Mensagem de erro: `461 * USER :Not enough parameters`

### Other Commands
- `JOIN`, `KICK`, `INVITE`, `TOPIC`, `MODE`, `PRIVMSG`, `OPER`,
> [!NOTE]
> This commands are not used in the registration process.
> In this case, send message: `451 * :You need to register before you can use that command`

<!-- `PART`,  `NAMES`, `LIST`,   `NOTICE`, `MOTD`, `LUSERS`, `VERSION`, `STATS`, `LINKS`, `TIME`, `CONNECT`, `TRACE`, `ADMIN`, `INFO`, `SERVLIST`, `SQUERY`, `WHO`, `WHOIS`, `WHOWAS`, `ISON` -->
