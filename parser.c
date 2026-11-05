#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

#include <fcntl.h>
#include <string.h>
#include "parser.h"

int parse_line(char *line, struct pipeline *pipe_line) {
    pipe_line->num_cmds = 0;
    pipe_line->bg = 0;

    char *token = strtok(line, " \t\r\n\a");
    if (!token) return 0;

    int cmd_idx = 0;
    int arg_idx = 0;
    pipe_line->cmds[0].infile = NULL;
    pipe_line->cmds[0].outfile = NULL;
    pipe_line->cmds[0].append = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            pipe_line->bg = 1;
        } else if (strcmp(token, "|") == 0) {
            pipe_line->cmds[cmd_idx].argv[arg_idx] = NULL;
            cmd_idx++;
            arg_idx = 0;
            pipe_line->cmds[cmd_idx].infile = NULL;
            pipe_line->cmds[cmd_idx].outfile = NULL;
            pipe_line->cmds[cmd_idx].append = 0;
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            pipe_line->cmds[cmd_idx].infile = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            pipe_line->cmds[cmd_idx].outfile = token;
            pipe_line->cmds[cmd_idx].append = 0;
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\r\n\a");
            pipe_line->cmds[cmd_idx].outfile = token;
            pipe_line->cmds[cmd_idx].append = 1;
        } else {
            pipe_line->cmds[cmd_idx].argv[arg_idx++] = token;
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    pipe_line->cmds[cmd_idx].argv[arg_idx] = NULL;
    pipe_line->num_cmds = (arg_idx > 0 || pipe_line->cmds[cmd_idx].infile) ? cmd_idx + 1 : cmd_idx;

    return pipe_line->num_cmds;
}
