#ifndef PARSER_H
#define PARSER_H
#include "shell.h"

#define MAX_COMMANDS 16

struct single_cmd {
    char *argv[MAXARG];
    char *infile;
    char *outfile;
    int append;
};

struct pipeline {
    struct single_cmd cmds[MAX_COMMANDS];
    int num_cmds;
    int bg;
};
int parse_line(char *line, struct pipeline *pipe_line);
#endif
