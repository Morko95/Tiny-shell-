#ifndef HANDLERS_H
#define HANDLERS_H
#include <signal.h>
#include "job_utils.h"
void pass_table(struct job_t *table);
void sigchild_handler(int sig);
void Sigchild_handler(void);
#endif
