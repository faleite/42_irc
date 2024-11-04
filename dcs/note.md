## Register user

### PASS
- [x] `PASS` must be the first or second command sent by the client.
  - [x] if `USER` and `NICK` are set first, send incorrect `PASS` and close the connection.
- [x] After sending `PASS`, `NICK`, and `USER`, an error message for `PASS` or a welcome message will be sent.
- [x] Incorrect `PASS`, the server must send an error message and close the connection.
- [x] After connecting, `PASS` and `USER` cannot be sent again.
  - [x] send message: :You may not reregister

### NICK
- [x] `NICK` can be sent at any time.
- [x] Error Messages:
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
- [x] `USER` process
[*RFC protocol*](https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3)\
Command: `USER`\
Parameters: `<username> <mode> <unused> <realname>`\
Example: `USER guest 0 * :Ronnie Reagan`
- [x] `USER` must have 4 parameters. The fourth parameter can contain spaces.
- [x] Error message: `461 * USER :Not enough parameters`

### Quit
- [x] `QUIT` process
[*RFC protocol*](https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.7)\
Command: `QUIT`\
Parameters: `<Quit Message>`\
Example: `QUIT :Gone to have lunch`

### HexChat
- To register a user in HexChat it sends:
  - **<<** `CAP` LS 302
  - **<<** `PASS` 1234
  - **<<** `NICK` fabricio
  - **<<** `USER` faleite 0 * :realname
    - `faleite` is the username
    - `0` is the user mode
    - `*` is the user's host
    - `:realname` is the user's real name

### Other Commands
- `JOIN`, `KICK`, `INVITE`, `TOPIC`, `MODE`, `PRIVMSG`, `OPER`, `LIST`
> [!NOTE]
> These commands are not used in the registration process.
> In this case, send message: `451 * :You need to register before you can use that command`