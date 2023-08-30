#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

pid_t padre, hijo; 
int i;

void ping_handler(int signum){
    printf("PING. Soy el hijo con PID: %d\n", getpid());
    sleep(1); 
    kill(padre, SIGUSR2);
}

void pong_handler(int signum){
    printf("PONG. Soy el padre con PID: %d\n", getpid()); 
    i++;
}

int main(){
    padre = getpid();

    signal(SIGUSR1, ping_handler);
    signal(SIGUSR2, pong_handler);

    hijo = fork(); 
    if(hijo == 0){
        while(1){};
    } else {
        printf("Soy el proceso padre y voy a empezar el ping pong.\n");
        i = 0;
        while(i < 3){
            kill(hijo, SIGUSR1);
            printf("El valor de i: %d\n", i); 
            sleep(1);
        };
    }
    kill(hijo, SIGKILL);
    return 0;
}