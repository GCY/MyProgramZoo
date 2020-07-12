/* example3.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int exit_status;
    pid_t PID = fork();

    switch(PID){
        case -1:
            perror("fork()");
            exit(-1);
        case 0:
            printf("[Child] I'm Child process\n");
            printf("[Child] Child's PID is %d\n", getpid());
            printf("[Child] Enter my exit status: ");
            scanf("%d", &exit_status);
            sleep(3);
            break;
        default:
            printf("[Parent] I'm Parent process\n");
            printf("[Parent] Parent's PID is %d\n", getpid());
            wait(&exit_status); // command here will get zombie
            // WEXITSTATUS is an macro
            printf("[Parent] Child's exit status is [%d]\n", WEXITSTATUS(exit_status));
    }

    return 0;
}
