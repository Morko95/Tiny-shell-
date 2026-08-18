#include <bits/types/sigset_t.h>
#include <signal.h>
#include <sys/types.h>
#include "job_utils.h"
#include "shell.h"
#include "signal_utils.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
void initialize(struct job_t *table){
    for(int i = 0; i<MAXJOBS; i++){
        table[i].pid = -1;
        table[i].pgid = -1;
        table[i].job_id = -1;
        table[i].state = UNDEF;
        table[i].cmdline[0] = '\0';
    }
}

void addjob(struct job_t *table,job_state_t state, pid_t pid, char *cmdline){
    for(int i=0; i<MAXJOBS; i++){
        if (table[i].pid == -1){
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
            table[i].pid = -1;
            table[i].pgid = -1;
            table[i].job_id = -1;
            table[i].state = UNDEF;
            table[i].cmdline[0] = '\0';
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
pid_t get_pid_index(pid_t pid, struct job_t *table){
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

void set_job_status(pid_t jid, struct job_t *table, job_state_t status){

    if((jid <MAXJOBS) && (table[jid].state != UNDEF)){
        table[jid].state = status;
    }

}
void fg_j(int jid, struct job_t *table){
    sigset_t prev_mask;
    sigset_t blocked = process_table_mask();
    sigprocmask(SIG_BLOCK, &blocked, &prev_mask);
    set_job_status(jid, table, FG);
    if(table[jid].state != UNDEF){
        pid_t pid = table[jid].pid;
        tcsetpgrp(STDIN_FILENO, pid);
        kill(-pid, SIGCONT);
        waitfg(pid, table);
        tcsetpgrp(STDIN_FILENO, getpgrp());
    }

    sigprocmask(SIG_SETMASK, &prev_mask, NULL);


}

void bg_j(int jid, struct job_t *table){
    sigset_t prev_mask;
    sigset_t blocked = process_table_mask();
    sigprocmask(SIG_BLOCK, &blocked, &prev_mask);
    if(jid<MAXJOBS){
        pid_t pid = table[jid].pid;
        set_job_status(jid, table, BG);
        kill(-pid, SIGCONT);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    }
}

void jobs(struct job_t *table){
        sigset_t mask, prev;
        sigemptyset(&mask);
        sigaddset(&mask, SIGCHLD);

        sigprocmask(SIG_BLOCK, &mask, &prev);

        for (int i = 0; i < MAXJOBS; i++) {
            if (table[i].pid != -1) {
                const char *state_str = (table[i].state == BG) ? "Running" :
                                        (table[i].state == STOPPED) ? "Stopped" : "Foreground";
                printf("[%d] (%d) %s %s\n", table[i].job_id, table[i].pid,
                                            state_str, table[i].cmdline);
            }
        }

        sigprocmask(SIG_SETMASK, &prev, NULL);
    }
