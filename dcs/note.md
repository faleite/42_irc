## Handle Channel

### Importante!
- Se o usuário usar o comando `JOIN` para um canal que existe, o server ignorará o comando.

### TASK
- [x] Channel name: `#foo`
  - first character need: `#`
- [x] Empty channel name
- [x] `JOIN 0`
- [x] `JOIN #foo,#bar`
- [x] `JOIN #foo,&foobar,#bar`
- [x] Channel name with space

### Command Join
**Command: `JOIN`**\
**Parameters:** `<channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"`
- Respostas numéricas:
  - [x] 461 ERR_NEEDMOREPARAMS
   - ex..: CMD `JOIN` sem parametros
  - [x] 403 ERR_NOSUCHCHANNEL
    - Usado para indicar que o nome do canal fornecido é inválido.
    - ex..: `<channel name> :No such channel`
	- ex..: `:ergo.test 403 user1 &foo :No such channel`
  - ERR_BANNEDFROMCHAN
  - ERR_INVITEONLYCHAN 
  - ERR_BADCHANNELKEY
  - ERR_CHANNELISFULL 
  - ERR_BADCHANMASK
  - ERR_TOOMANYCHANNELS
  - ERR_TOOMANYTARGETS 
  - ERR_UNAVAILRESOURCE
  - RPL_TOPIC
- Examples: **(Sugestao nao lidar com pass)**
  - **JOIN #foobar ; Comando para entrar no canal #foobar.**
  - **JOIN 0 ; Deixar todos os canais atualmente conectado.**
    - ex..: `:araujo!~u@bpnicdmf5m7vu.irc PART #global`
    - ex..: `:ergo.test NOTICE araujo :Warning: /JOIN 0 will remove you from all channels. To confirm, type: /JOIN 0 cjxx8`
  - *JOIN #foo,#bar ​​fubar,foobar ;*
    - *Comando para entrar no canal #foo usando a pass "fubar"\
	 e no canal #bar usando a pass "foobar".*
  - **JOIN #foo,#bar ​​; Comando para entrar nos canais #foo e #bar.**
  - **:WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone;**\
    **JOIN mensagem do WiZ no canal #Twilight_zone**
	- Quando um usuario entra em um canal os mebros deste canal recebe uma menssagem\
  informando que o usuario entrou no canal. `:user2!~u@bpnicdmf5m7vu.irc JOIN #foo`

### Test
```sh
pass 1234
nick user1
user a a a a
```
```sh
pass 1234
nick user2
user b b b b
```