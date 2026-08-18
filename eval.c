#include "parser.h"
#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"
#include "eval.h"
#include "job_utils.h"
#include "main_routine_signals.h"
#include "signal_utils.h"

void evaluate(char *line, struct job_t *table){
    sigset_t prevmask;
    sigset_t blocked_for_jobs = process_table_mask();
    int bg = 0;
    char **argv = parse(line, &bg);
    if(argv[0] == NULL){
        free(argv);
        return;
    }
    else if(strcmp(argv[0], "cd") == 0){
        char *target = argv[1] ? argv[1]: getenv("HOME");
        int new_env = chdir(target);
        if(new_env<0){
            perror("Cannot change the directory");
        }
        free(argv);
        return;
    }
    else if (strcmp(argv[0], "exit") == 0){
        int status = 0;
        if(argv[1]){
           status = atoi(argv[1]);
        }
        free(argv);
        exit(status);

    }
    else if (strcmp(argv[0], "pwd") == 0){
        char *curr_env = getcwd(NULL, 0);
        if (curr_env == NULL){
            perror("pwd command failed");
            free(argv);
            return;
        }
        printf("%s", curr_env);
        free(argv);
        free(curr_env);
        return;
    }
    else if (strcmp(argv[0], "jobs") == 0){
        jobs(table);
        free(argv);
    }
    else if(strcmp(argv[0], "bg") == 0){
        if (argv[1] == NULL){
            printf("Not enough arguments");
            free(argv);
        }
        else{
            if(argv[1][0] == '%'){
                pid_t jid = atoi(&argv[1][1]);
                bg_j(jid, table);
                free(argv);
            }
            else{
                pid_t pid = atoi(&argv[1][1]);
                pid_t jid = get_pid_index(pid, table);
                bg_j(jid, table);
                free(argv);
            }
        }
    }
    else if(strcmp(argv[0], "fg") == 0){
            if (argv[1] == NULL){
                printf("Not enough arguments");
                free(argv);
            }
            else{
                if(argv[1][0] == '%'){
                    pid_t jid = atoi(&argv[1][1]);
                    fg_j(jid, table);
                    free(argv);
                }
                else{
                    pid_t pid = atoi(&argv[1][1]);
                    pid_t jid = get_pid_index(pid, table);
                    fg_j(jid, table);
                    free(argv);
                }
            }
        }
    else {
        sigprocmask(SIG_BLOCK, &blocked_for_jobs, &prevmask);
        pid_t pid = fork();
        int status;
        switch (pid){
            case -1:
                perror("Fork error");
                return;
            case 0:
                sigprocmask(SIG_SETMASK, &prevmask, NULL);
                default_child_signals();
                setpgid(0, 0);
                if(execvp(argv[0], argv) == -1){
                    perror("execvp failed");
                    exit(EXIT_FAILURE);
                }
            default:
                setpgid(pid, pid);
                if (bg != IS_BG){
                    addjob(table, FG, pid, *argv);
                    tcsetpgrp(STDIN_FILENO, pid);
                    waitfg(pid, table);
                    tcsetpgrp(STDIN_FILENO, getpgrp());
                }
                else{
                    addjob(table, BG, pid, *argv);
                }
                sigprocmask(SIG_SETMASK, &prevmask, NULL);
                break;

        }


        }
    }
