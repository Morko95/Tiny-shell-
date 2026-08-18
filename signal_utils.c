#include <signal.h>
#include <bits/types/sigset_t.h>
#include "signal_utils.h"

sigset_t process_table_mask(void){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    return set;
}
