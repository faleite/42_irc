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
- [ ] MSG Error:
  - [ ] `431 * :No nickname given`
  - [ ] `432 * <nick> :Erroneous nickname`
  - [x] `433 * <nick> :Nickname is already in use`
  - [ ] `437 * <nick> :Nick/channel is temporarily unavailable`

```cpp
// Returned when a nickname parameter expected for a
//            command and isn't found
std::string  Replies::ERR_NONICKNAMEGIVEN()
{
  return(":jf.irc 431 * :No nickname given");
}

/*
 Returned after receiving a NICK message which contains
 characters which do not fall in the defined set.  See
 section 2.3.1 for details on valid nicknames.
*/
std::string  Replies::ERR_ERRONEUSNICKNAME()
{
  return(":jf.irc 432 * <nick> :Erroneous nickname");
}

/*
437    ERR_UNAVAILRESOURCE
              "<nick/channel> :Nick/channel is temporarily unavailable"

         - Returned by a server to a user trying to join a channel
           currently blocked by the channel delay mechanism.

         - Returned by a server to a user trying to change nickname
           when the desired nickname is blocked by the nick delay
           mechanism.
*/
```

### USER
[*RFC protocol*](https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3)\
Comando: `USER`\
Parâmetros: `<username> <mode> <unused> <realname>`\
Exemplo: `USER guest 0 * :Ronnie Reagan`
- [ ] `USER` deve ter 4 parâmetros. O quarto parâmetro pode ter espaços.
- [ ] Mensagem de erro: `461 * USER :Not enough parameters`

### Quit
[*RFC protocol*](https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.7)\
Comando: `QUIT`\
Parâmetros: `<Quit Message>`\
Exemple: `QUIT :Gone to have lunch`

### Other Commands
- `JOIN`, `KICK`, `INVITE`, `TOPIC`, `MODE`, `PRIVMSG`, `OPER`,
> [!NOTE]
> This commands are not used in the registration process.
> In this case, send message: `451 * :You need to register before you can use that command`

<!-- `PART`,  `NAMES`, `LIST`,   `NOTICE`, `MOTD`, `LUSERS`, `VERSION`, `STATS`, `LINKS`, `TIME`, `CONNECT`, `TRACE`, `ADMIN`, `INFO`, `SERVLIST`, `SQUERY`, `WHO`, `WHOIS`, `WHOWAS`, `ISON` -->
