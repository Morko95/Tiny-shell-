#include "parser.h"
#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"
#include "eval.h"
#include "job_utils.h"
#include "main_routine_signals.h"
#include "signal_utils.h"

static void setup_redirections(struct single_cmd *cmd) {
    if (cmd->infile) {
        int fd_in = open(cmd->infile, O_RDONLY);
        if (fd_in < 0) {
            perror("open infile");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (cmd->outfile) {
        int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
        int fd_out = open(cmd->outfile, flags, 0644);
        if (fd_out < 0) {
            perror("open outfile");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
}

void evaluate(char *line, struct job_t *table) {
    struct pipeline pl;
    char line_backup[MAXARG];
    strncpy(line_backup, line, sizeof(line_backup));

    if (parse_line(line, &pl) == 0) return;

    // Builtiny obsługiwane tylko wtedy, gdy nie tworzymy potoku
    if (pl.num_cmds == 1 && pl.cmds[0].argv[0] != NULL) {
        char **argv = pl.cmds[0].argv;

        if (strcmp(argv[0], "cd") == 0) {
            char *target = argv[1] ? argv[1] : getenv("HOME");
            if (chdir(target) < 0) {
                perror("Cannot change directory");
            }
            return;
        }
        else if (strcmp(argv[0], "exit") == 0) {
            int status = argv[1] ? atoi(argv[1]) : 0;
            exit(status);
        }
        else if (strcmp(argv[0], "pwd") == 0) {
            char *curr_env = getcwd(NULL, 0);
            if (curr_env == NULL) {
                perror("pwd command failed");
                return;
            }
            printf("%s\n", curr_env);
            free(curr_env);
            return;
        }
        else if (strcmp(argv[0], "jobs") == 0) {
            jobs(table);
            return;
        }
        else if (strcmp(argv[0], "bg") == 0) {
            if (argv[1] == NULL) {
                printf("Not enough arguments\n");
            } else {
                pid_t jid = (argv[1][0] == '%') ? atoi(&argv[1][1]) : get_pid_index(atoi(argv[1]), table);
                bg_j(jid, table);
            }
            return;
        }
        else if (strcmp(argv[0], "fg") == 0) {
            if (argv[1] == NULL) {
                printf("Not enough arguments\n");
            } else {
                pid_t jid = (argv[1][0] == '%') ? atoi(&argv[1][1]) : get_pid_index(atoi(argv[1]), table);
                fg_j(jid, table);
            }
            return;
        }
    }

    sigset_t prevmask, blocked_for_jobs = process_table_mask();
    sigprocmask(SIG_BLOCK, &blocked_for_jobs, &prevmask);

    int in_fd = STDIN_FILENO;
    int pipefd[2];
    pid_t first_pid = 0;

    for (int i = 0; i < pl.num_cmds; i++) {
        if (i < pl.num_cmds - 1) {
            if (pipe(pipefd) < 0) {
                perror("pipe error");
                break;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork error");
            break;
        }

        if (pid == 0) { // Dziecko
            sigprocmask(SIG_SETMASK, &prevmask, NULL);
            default_child_signals();

            setpgid(0, first_pid == 0 ? 0 : first_pid);

            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (i < pl.num_cmds - 1) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            setup_redirections(&pl.cmds[i]);

            if (execvp(pl.cmds[i].argv[0], pl.cmds[i].argv) < 0) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }

        // Rodzic
        if (first_pid == 0) first_pid = pid;
        setpgid(pid, first_pid);

        if (in_fd != STDIN_FILENO) close(in_fd);
        if (i < pl.num_cmds - 1) {
            close(pipefd[1]);
            in_fd = pipefd[0];
        }
    }

    if (pl.bg != IS_BG) {
        addjob(table, FG, first_pid, line_backup);
        tcsetpgrp(STDIN_FILENO, first_pid);
        waitfg(first_pid, table);
        tcsetpgrp(STDIN_FILENO, getpgrp());
    } else {
        addjob(table, BG, first_pid, line_backup);
    }

    sigprocmask(SIG_SETMASK, &prevmask, NULL);
}
