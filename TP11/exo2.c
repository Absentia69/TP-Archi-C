#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int count = 0;

void sighand(int sig){
    printf("Signal reçu:%d\n", sig);

    count++;
    printf("Signal SIGINT %d fois:\n", count);

    if (count >= 5){
        signal(SIGINT, SIG_DFL);
    }
    
}


int main(int argc, char const *argv[])
{
    printf("PID:%d\n", getpid());

   signal(SIGINT, sighand);
    
    while (1){
        sleep(5);
    }
    
    return 0;
}



