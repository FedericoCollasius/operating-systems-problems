#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int N; // Cantidad de procesos a crear < 10 
int J; // 0 <= Numero maldito < N  
int generate_random_number(){
	return (rand() % N);
}

void sigterm_handler(int signum){
  sleep(1);
  int randnum = generate_random_number();
  printf("El numero random del proceso %d: %d\n", getpid(), randnum);
  pid_t pid = getpid();
  if(randnum == J){
    printf("Soy el proceso %d y me estoy por morir\n", pid); 
    exit(1);
    }
}

void sigchild_handler(int signum){
    sleep(1); 
    printf("Soy el proceso padre y voy a matar al proceso hijo");
    kill(SIGKILL, );
}



int main(int argc, char const *argv[]){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL));
    
    N = atoi(argv[1]);
	int K = atoi(argv[2]); // Cantidad de rondas  
	J = atoi(argv[3]);

    // Completar
    signal(SIGTERM, sigterm_handler); 
    signal(SIGCHLD, sigchild_handler);

    pid_t pid;  
    pid_t childs[N];
    
    for(int i = 0; i < N; i++){
        sleep(1);
        pid = fork();
        if(pid == 0){
            printf("Soy el proceso: %d\n", getpid()); 
            while(1);
        } else {
            childs[i] = pid;
        }
    }

    for(int i = 0; i < K; i++){
        sleep(1);
        printf("Soy el proceso padre y esta es la ronda %d\n", i);
        for(int j = 0; j < N; j++){
            sleep(1);
            pid = childs[j];
            printf("Le mando una señal a mi hijo con pid: %d\n", pid);
            kill(pid, SIGTERM); 
        }   
    }

    for(int i = 0; i < N; i++){
        pid = childs[i];
        if (pid != -1){
            printf("El proceso con id %d sobrevivio!\n", pid); 
            kill(pid, SIGKILL); 
        }
    }

    return 0; 
}