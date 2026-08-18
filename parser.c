#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "parser.h"

char **parse(char *line, int *bg) {
    char **args = malloc(MAXARG * sizeof(char *));
    if (!args) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    *bg = 0;
    int position = 0;
    char *token = strtok(line, " \n\t\r\a");

    while (token != NULL && position < MAXARG - 1) {
        args[position++] = token;
        token = strtok(NULL, " \n\t\r\a");
    }
    args[position] = NULL;

    if (position == 0) {
        return args; // Pusta linia
    }

    if (strcmp(args[position - 1], "&") == 0) {
        *bg = IS_BG;
        args[position - 1] = NULL; // Usuwamy '&', by execvp go nie przetwarzał
    }

    return args;
}
