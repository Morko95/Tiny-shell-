#ifndef JOB_UTILS_H
#define JOB_UTILS_H
#include <sys/types.h>
#include "shell.h"
typedef enum {UNDEF, FG, BG, STOPPED} job_state_t;
struct job_t{
    pid_t pid;
    pid_t pgid;
    pid_t job_id;
    job_state_t state;
    char cmdline[MAXARG];
    };
void initialize(struct job_t *jobtable);
void addjob(struct job_t *table,job_state_t state, pid_t pid, char *cmdline);
void clearjob(struct job_t *table, pid_t pid);
void stop_job(struct job_t *table, pid_t pid);
pid_t get_pid_index(pid_t pid, struct job_t *table);
void waitfg(pid_t pid, struct job_t *table);
void set_job_status(pid_t jid, struct job_t *table, job_state_t status);
void fg_j(int jid, struct job_t *table);
void bg_j(int jid, struct job_t *table);
void jobs(struct job_t *table);
#endif
