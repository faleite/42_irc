# PS1="\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\\$ "
# PS1='\e[35m[\e[92m\u\e[35m]\e[0m$ '
# PS1='\e[92m\u\e[0m@\e[94m\h\e[0m:\e[35m\w\e[0m$ '
PS1='\e[35m[\e[92m\u\e[35m]\e[0m:\e[34m\w\e[0m$ '
alias comp='c++ -Wall -Wextra -Werror -std=c++98 -g ./server/Server.cpp ./client/Client.cpp main.cpp'
# alias crun='c++ -Wall -Wextra -Werror -std=c++98 -g ./server/Server.cpp ./client/Client.cpp main.cpp && ./a.out'
alias mrun='make re && ./ircserv 6667 1234'
alias rma='rm ./a.out'
alias rua='./a.out'
alias mf='make fclean'
alias valg='make re && valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv 6667 1234'
alias client='nc localhost 6667'