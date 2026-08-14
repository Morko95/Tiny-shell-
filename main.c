#include <stdio.h>
#include <stdlib.h>
#include <bits/types/sigset_t.h>
#include "read_input.h"
#include "eval.h"
#include "main_routine_signals.h"
int main(){
    default_shell_signals();
    while(1){
       printf("tsh>");
       fflush(stdout);
       char *input_line = read_input();
       if (input_line == NULL){
           continue;
       }

       evaluate(input_line);
       free(input_line);

   }
}
