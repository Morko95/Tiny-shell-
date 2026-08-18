#include <bits/types/sigset_t.h>
#include <signal.h>
#include <sys/types.h>
#include "job_utils.h"
#include "shell.h"
#include <string.h>
void initialize(struct job_t *table){
    for(int i = 0; i<MAXJOBS; i++){
        table[i].pid = 0;
        table[i].pgid = 0;
        table[i].job_id = 0;
        table[i].state = UNDEF;
        table[i].cmdline[0] = '\0';
    }
}

void addjob(struct job_t *table,job_state_t state, pid_t pid, char *cmdline){
    for(int i=0; i<MAXJOBS; i++){
        if (table[i].pid == 0){
            table[i].pid = pid;
            table[i].pgid = pid;
            table[i].job_id = i;
            table[i].state = state;
            strcpy(table[i].cmdline, cmdline);
        }
    }
}

void clear_job(struct job_t *table, pid_t pid){
    for (int i = 0; i<MAXJOBS; i++){
        if (table[i].pid == pid){
            table[i].pid = 0;
            table[i].pgid = 0;
            table[i].job_id = 0;
            table[i].state = UNDEF;
        }
    }
}
void stop_job(struct job_t *table, pid_t pid){
    for (int i = 0; i<MAXJOBS; i++){
        if(table[i].pid == pid){
            table[i].state = STOPPED;
        }
    }
}
//-1 if no pid, defaultly index in job table
int get_pid_index(pid_t pid, struct job_t *table){
    for(int i = 0; i<MAXJOBS; i++){
        if(table[i].pid == pid){
            return i;
        }
    }
    return -1;
}
void waitfg(pid_t pid, struct job_t *table){
    int index = get_pid_index(pid, table);
    sigset_t mask;
    if(index == -1){
        return;
    }
    sigemptyset(&mask);
    while(table[index].state == FG){
        sigsuspend(&mask);
    }
}
