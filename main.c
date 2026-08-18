#include <stdio.h>
#include <stdlib.h>
#include <bits/types/sigset_t.h>
#include "job_utils.h"
#include "read_input.h"
#include "eval.h"
#include "main_routine_signals.h"
#include "shell.h"
#include "handlers.h"
int main(){
    struct job_t jobs_table[MAXJOBS];
    Sigchild_handler();
    initialize(jobs_table);
    pass_table(jobs_table);
    default_shell_signals();
    while(1){
       printf("tsh>");
       fflush(stdout);
       char *input_line = read_input();
       if (input_line == NULL){
           continue;
       }

       evaluate(input_line, jobs_table);
       free(input_line);

   }
}
