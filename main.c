#include <stdio.h>
#include <stdlib.h>
#include "read_input.h"

int main(){
    while(1){
       printf("tsh>");
       fflush(stdout);
       char *input_line = read_input();
       if (input_line == NULL){
           continue;
       }

   }
}
