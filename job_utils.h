#ifndef JOB_UTILS_H
#define JOB_UTILS_H
#include <sys/types.h>
#include "shell.h"
typedef enum {UNDEF, FG, BG, ST} job_state_t;
struct job_t{
    pid_t pid;
    pid_t pgid;
    pid_t job_id;
    job_state_t state;
    char cmdline[MAXLINE];
    };
void initialize(struct job_t *jobtable);
#endif
