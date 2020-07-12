/* example4.c */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

static void sig_handler(int sig){
    int retval;
    
    if ( sig == SIGCHLD ){ 
        // 等待子程序的結束狀態
        wait(&retval);
        
        printf("CATCH SIGNAL PID=%d\n",getpid());
    }
}

int main(){
    int exit_status;

    // 呼叫 signal 來接收 SIGCHLD 信號
    signal(SIGCHLD,sig_handler);

    // 從呼叫 fork 開始, 會分成兩支程序多工進行
    pid_t PID = fork();

    switch(PID){
        // PID == -1 代表 fork 出錯
        case -1:
            perror("fork()");
            exit(-1);

        // PID == 0 代表是子程序
        case 0:
            printf("[Child] I'm Child process\n");
            printf("[Child] Child's PID is %d\n", getpid());
            sleep(5); // 暫停 5 秒
            break;

        // PID > 0 代表是父程序
        default:
            printf("[Parent] I'm Parent process\n");
            printf("[Parent] Parent's PID is %d\n", getpid());
            // wait(&exit_status);
    }

    return 0;
}
