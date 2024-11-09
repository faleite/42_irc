## Handle Channel

### TASK
- [x] Channel name: `#foo`
  - first character need: `#`
- [x] Empty channel name
- [x] `JOIN 0`
- [x] `JOIN #foo,#bar`
- [x] `JOIN #foo,&foobar,#bar`
- [x] Channel name with space
- [x] Remove the message when starting connection (line: 39, file: connectRegister.cpp): `<user_name> Has Join to the server`
- [x] Remove code line **99** and **100** on file: `Channelcpp`
- [x] If the user uses the `JOIN` command for an existing channel, the server will ignore the command.
- [x] Replies:
  - [x] 461 ERR_NEEDMOREPARAMS
   - ex..: CMD `JOIN` without parameters
  - [x] 403 ERR_NOSUCHCHANNEL
    - Used to indicate that the provided channel name is invalid.
    - ex..: `<channel name> :No such channel`
    - ex..: `:ergo.test 403 user1 &foo :No such channel`
  - [x] ERR_CHANNELISFULL 
  - [x] ERR_INVITEONLYCHAN 
  - [x] ERR_BADCHANNELKEY
  - [x] ERR_USERONCHANNEL

### Command Join
**Command: `JOIN`**\
**Parameters:** `<channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"`
- Numeric replies:
  - [ ] ERR_BANNEDFROMCHAN
  - [ ] ERR_BADCHANMASK
  - [ ] ERR_TOOMANYCHANNELS
  - [ ] ERR_TOOMANYTARGETS 
  - [ ] ERR_UNAVAILRESOURCE
  - [ ] RPL_TOPIC
- Examples: **(Suggestion not to handle pass)**
  - **JOIN #foobar ; Command to join the channel #foobar.**
  - **JOIN 0 ; Leave all currently connected channels.**
    - ex..: `:araujo!~u@bpnicdmf5m7vu.irc PART #global`
    - ex..: `:ergo.test NOTICE araujo :Warning: /JOIN 0 will remove you from all channels. To confirm, type: /JOIN 0 cjxx8`
  - *JOIN #foo,#bar ​​fubar,foobar ;*
    - *Command to join the channel #foo using the pass "fubar"\
     and the channel #bar using the pass "foobar".*
  - **JOIN #foo,#bar ​​; Command to join the channels #foo and #bar.**
  - **:WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone;**\
    **JOIN message from WiZ in the channel #Twilight_zone**
    - When a user joins a channel, the members of that channel receive a message\
  informing that the user has joined the channel. `:user2!~u@bpnicdmf5m7vu.irc JOIN #foo`

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