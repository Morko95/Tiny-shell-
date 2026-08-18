#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H
#include <bits/types/sigset_t.h>
#include <signal.h>
sigset_t process_table_mask(void);
#endif
