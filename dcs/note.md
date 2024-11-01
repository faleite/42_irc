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