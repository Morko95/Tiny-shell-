# Shell roadmap

## main.c
 - [x] repl loop with process and signal handling
 - [x] read a line from stdin
 - [x] parse the input
 - [x] evaluate the input
 - [ ] support pipes and redirections
 - [ ] support job control
## reading input
 - [x] write a input reading function
 - [x] handle eof exiting and getline error
 - [ ] modify the function to reuse the buffer
## parsing input
 - [x] write a parser
 - [ ] review the behaviour when arg number is greater than ARG_MAX
## evaluating input
 - [x] write an eval function that evaluates parsed input
 - [x] support cd with chdir
 - [x] support pwd
 - [x] support exit
 - [ ] refactor with command struct for every command
 - [ ] dive into execvp failed spec
 - [x] support not built in commands
 - [ ] refactor if-else block with a dispatch table
 ## signals
  - [x] handle ctrl c and ctrl z in main program
## job control
  
 ## misc
  - [ ] fix printing newline
  - [ ] add current directory
