#include <stdio.h>
#include<stdlib.h>
#include "read_input.h"
char *read_input(void){
    //TODO not allocate everything each call, pass params in main
    //
    size_t bufsize = 0;
    ssize_t characters_read;
    char* line = NULL;
    characters_read = getline(&line, &bufsize, stdin);
    if (characters_read == -1){
        if (feof(stdin)){
            free(line);
            exit(EXIT_SUCCESS);
        }
        else{
            perror("tshell: read error");
            free(line);
            return(NULL);
        }
    }
    return line;
}
