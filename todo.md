# Shell roadmap

## main.c
 - [ ] repl loop with process and signal handling
 - [x] read a line from stdin
 - [ ] parse the input
## reading input
 - [x] write a input reading function
 - [x] handle eof exiting and getline error
 - [ ] modify the function to reuse the buffer
## parsing input
 - [x] write a parser
 - [ ] review the behaviour when arg number is greater than ARG_MAX
## evaluating input
 - [ ] write an eval function that evaluates parsed input
