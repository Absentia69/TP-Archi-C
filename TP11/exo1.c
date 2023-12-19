#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sighand(int sig){
    printf("Signal reçu:%d", sig);
}

int main(int argc, char const *argv[])
{
    printf("PID:%d\n", getpid());

   for (int i = 1; i < NSIG; i++){
        signal(i, SIG_IGN);
    }
    
   for (int i = 0; i < NSIG; i++){
        signal(i, sighand);
    }
    
    while (1){
        sleep(5);
    }
    
    return 0;
}



