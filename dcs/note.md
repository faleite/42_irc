## Handle Channel

### JOIN New Channel
```sh
<< JOIN #local
>> :faaraujo!~u@e77ncepu88yiy.irc JOIN #local * realname
<< MODE #local
<< WHO #local %chtsunfra,152
>> :ergo.test 353 faaraujo = #local :@faaraujo!~u@e77ncepu88yiy.irc
>> :ergo.test 366 faaraujo #local :End of NAMES list
>> :ergo.test 324 faaraujo #local +Cnt
>> :ergo.test 329 faaraujo #local 1731002626 # Not avaliable in RFC 2812
>> :ergo.test 354 faaraujo 152 #local ~u e77ncepu88yiy.irc ergo.test faaraujo H@ 0 realname
>> :ergo.test 315 faaraujo #local :End of WHO list
```

### JOIN Existing Channel
```sh
<< JOIN #local
>> :juan!~u@e77ncepu88yiy.irc JOIN #local * JaunP
<< MODE #local
<< WHO #local %chtsunfra,152
>> :ergo.test 353 juan = #local :@faaraujo!~u@e77ncepu88yiy.irc juan!~u@e77ncepu88yiy.irc
>> :ergo.test 366 juan #local :End of NAMES list
>> :ergo.test 324 juan #local +Cnt
>> :ergo.test 329 juan #local 1731007297
>> :ergo.test 354 juan 152 #local ~u e77ncepu88yiy.irc ergo.test faaraujo H@ 0 realname
>> :ergo.test 354 juan 152 #local ~u e77ncepu88yiy.irc ergo.test juan H 0 JaunP
>> :ergo.test 315 juan #local :End of WHO list
```

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