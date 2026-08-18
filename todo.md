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
  - [x] implement a job_t structure
  - [x] add addjob() and clearjob() functions to manipulate job table
  - [ ] add jobs builtin command to display the job table state
  - [ ] seperate the waitpid() into the sigchild handler
  - [ ] implement waitfg() that waits for foreground process to terminate
  - [x] review the background and foreground process launching
 ## misc
  - [ ] fix printing newline
  - [ ] add current directory
