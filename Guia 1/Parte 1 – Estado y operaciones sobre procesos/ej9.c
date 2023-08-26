#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

pid_t padre, hijo; 

void ping_handler(int signum){
    printf("PING %d\n", getpid());
    sleep(1); 
    kill(padre, SIGUSR2);
}

void pong_handler(int signum){
    printf("PONG %d\n", getpid());
    sleep(1); 
    kill(hijo, SIGUSR1);
}

int main(){
    padre = getpid();

    signal(SIGUSR1, ping_handler);
    signal(SIGUSR2, pong_handler);

    hijo = fork(); 
    if(hijo == 0){
        sleep(1);
        printf("Soy el proceso hijo\n");
        while(1){};
    } else {
        printf("Soy el proceso padre y voy a empezar el ping pong\n");
        for(int i = 0; i < 3; i++){
            printf("Le mando una señal al proceso hijo\n");
            kill(hijo, SIGUSR1);
            sleep(1); 
        }
    }
    return 0;
}