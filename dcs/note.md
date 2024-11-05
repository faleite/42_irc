## Handle Channel

### Command Join
- Command: `JOIN`
- Parameters: \<channel\> *( "," \<channel\> ) [ \<key\> *( "," \<key\> ) ] ) / "0"
- Respostas numéricas:
  - 461 ERR_NEEDMOREPARAMS
   - ex..: CMD `JOIN` sem parametros
  - ERR_BANNEDFROMCHAN
  - ERR_INVITEONLYCHAN 
  - ERR_BADCHANNELKEY
  - ERR_CHANNELISFULL 
  - ERR_BADCHANMASK
  - 403 ERR_NOSUCHCHANNEL
    - Usado para indicar que o nome do canal fornecido é inválido.
    - ex..: `<channel name> :No such channel`
	- ex..: `:ergo.test 403 user1 &foo :No such channel`
  - ERR_TOOMANYCHANNELS
  - ERR_TOOMANYTARGETS 
  - ERR_UNAVAILRESOURCE
  - RPL_TOPIC
- Examples: **(Sugestao nao lidar com pass)**
  - **JOIN #foobar ; Comando para entrar no canal #foobar.**
  - **JOIN 0 ; Deixar todos os canais atualmente conectado.**
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

### Join validate
- Channel name: `#foo`
  - first character need: `#`