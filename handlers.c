#include "handlers.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "job_utils.h"
static struct job_t *ref_table = NULL;
void pass_table(struct job_t *table){
    ref_table = table;
}
void sigchild_handler(int sig){
    int saved_errno = errno;
    int status;
    pid_t pid;
    while((pid =waitpid(-1, &status, WUNTRACED  | WNOHANG ))> -1){
        if(WIFEXITED(status)){
            clearjob(ref_table,  pid);
        }
        if(WIFSTOPPED(status)){
            stop_job(ref_table,  pid);
        }
    }
    errno = saved_errno;

}
//wrapper for sigchild_handler()
void Sigchild_handler(void){
    sigset_t set;
    struct sigaction sa;
    sa.sa_handler = sigchild_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL)== -1){
        exit(EXIT_FAILURE);
    }
}
