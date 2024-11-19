## Handle KICK and PART commands

## TESTINGS

### PART command
***expected behavior:***
```sh
<< PART #local :Leaving
>> :ka100 353 faleite = #local : // not mandatory
>> :ka100 366 faleite #local :End of /WHO list. // not mandatory
>> :faleite!araujo@localhost PART #local :Leaving
```
***inside channel on Hexchat:***
```sh
msg 1. * Now talking on #local
       * Users on #local: @faleite // not mandatory

msg 2. * Users on #local: // not mandatory
       * You have left channel #local (Leaving)
```
---

***jf.irc behavior:***
```sh
<< PART #local :Leaving
>> :jf.irc KICK #local faleite :Client has leave
```
***jf.irc inside channel on Hexchat:***
```sh
msg 1. * Now talking on #local

msg 2. * You have been kicked from #local by jf.irc (Client has leave)
```

### KICK command
***expected behavior:***
```sh
<< KICK #local faleite :sair
>> :faleite!araujo@localhost KICK #local faleite :sair
>> :faleite!araujo@localhost KICK #local faleite :sair
>> :ka100 353 faleite = #local :
>> :ka100 366 faleite #local :End of /WHO list.
```
***inside channel on Hexchat:***
```sh
msg 1. * Now talking on #local
       * Users on #local: @faleite 

msg 2. * You have been kicked from #local by faleite (sair)
```
---

***jf.irc behavior:***
```sh
<< KICK #local faleite :sair 
>> :jf.irc KICK #local faleite :You have been kicked from the channel
>> ::jf.irc KICK #local faleite :Client has been kicked
```
***jf.irc inside channel on Hexchat:***
```sh
msg 1. * Now talking on #local

msg 2. * You have been kicked from #local by jf.irc (You have been kicked from the channel)
```