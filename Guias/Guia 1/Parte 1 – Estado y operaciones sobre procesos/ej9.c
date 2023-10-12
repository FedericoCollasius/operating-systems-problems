#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

pid_t padre, hijo; 
int i;
int received = 0;
char choice;

void ping_handler(int signum){
    printf("PING. Soy el hijo con PID: %d\n", getpid());
    sleep(1); 
    kill(padre, SIGUSR2);
}

void pong_handler(int signum){
    printf("PONG. Soy el padre con PID: %d\n", getpid()); 
    i++;
    received = 1;
}

int main(){
    padre = getpid();

    signal(SIGUSR1, ping_handler);
    signal(SIGUSR2, pong_handler);

    do {
        hijo = fork(); 
        if(hijo == 0){
            while(1){};
        } else {
            printf("Soy el proceso padre y voy a empezar el ping pong.\n");
            i = 0;
            kill(hijo, SIGUSR1);
            while(i < 3){
                while(received == 0){};
                received = 0;
                if (i < 3) {
                    kill(hijo, SIGUSR1);
                }
            }
            kill(hijo, SIGKILL);
            wait(NULL); // Espera a que el hijo termine
            printf("¿Queres terminar la ejecución? (s/n): ");
            scanf(" %c", &choice);
        }
    } while (choice != 's' && choice != 'S');

    return 0;
}
