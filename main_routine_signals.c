#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "main_routine_signals.h"
void default_shell_signals(void){
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGINT, &sa, NULL)==-1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGQUIT, &sa, NULL)==-1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGSTOP, &sa, NULL) == -1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

}
void default_child_signals(void){
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    if(sigaction(SIGINT, &sa, NULL)==-1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGSTOP, &sa, NULL) == -1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
    if(sigaction(SIGSTOP, &sa, NULL) == -1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
}
