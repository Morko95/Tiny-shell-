#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
char **parse(char *line, int *bg){
    char **args = malloc(MAXARG*sizeof(char *));
    if (!args){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    int position = 0;
    char *token = strtok(line, " \n\t\r\a");
    while (token!=NULL){
        if(position == ARG_MAX){
            break;
        }
        args[position] = token;
        position +=1;
        token = strtok(NULL, " \n\t\r\a");
    }
    args[position] = NULL;
    position--;
    if (position > 0 && strcmp(args[position - 1], "&") == 0) {
        *bg = IS_BG;
    }
    return args;

}
