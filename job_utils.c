#include <sys/types.h>
#include "job_utils.h"
#include "shell.h"
void initialize(struct job_t *table){
    for(int i = 0; i<MAXJOBS; i++){
        table[i].pid = 0;
        table[i].pgid = 0;
        table[i].job_id = 0;
        table[i].state = UNDEF;
        table[i].cmdline[0] = '\0';
    }
}
