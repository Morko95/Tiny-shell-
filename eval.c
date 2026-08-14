#include "parser.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "eval.h"
#include "main_routine_signals.h"
void evaluate(char *line){
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
    //TODO LATER
    /*else if( strcmp(argv[0], "bg") == 0){
        if (bg&IS_BG){
            printf("The process already running in the background");
            free(argv);
            return;
            }*/

    //}
    else {
        pid_t pid = fork();
        int status;
        switch (pid){
            case -1:
                perror("Fork error");
                return;
            case 0:
                default_child_signals();
                if(execvp(argv[0], argv) == -1){
                    perror("execvp failed");
                    exit(EXIT_FAILURE);
                }
            default:
                if(waitpid(pid, &status, 0) == -1){
                    perror("Waitpid failed");
                    exit(EXIT_FAILURE);
                }

                break;

        }


        }
    }
