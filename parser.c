#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include <stdlib.h>
#include "parser.h"
char **parse(char *line){
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
    return args;

}
