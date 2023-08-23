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

pid_t childs[9];

void sigterm_handler(int signum){
  sleep(3);
  pid_t pid = getpid();
  int randnum = generate_random_number();
  printf("El numero random del proceso %d: %d\n", getpid(), randnum);
  if(randnum == J){
    printf("Soy el proceso %d y me estoy por morir\n", pid); 
    exit(0);
    }
}

void sigchild_handler(int signum){
    pid_t pid;
    int status;
    // La función waitpid con la opción WNOHANG permite al proceso padre "consultar" la tabla de procesos 
    // y ver si hay alguna entrada de un proceso hijo que ha terminado pero cuyo estado aún no ha sido recogido. 
    // Si encuentra uno, lo recoge y retorna su PID y entro al while. Si no encuentra ninguno, simplemente retorna 0
    // y no entro. Cada vez que paso por el ciclo verifico si hay un hijo que terminó. 
    // El while es porque puede haber mas de un hijo que cambio de estado. Con un if 
    // corro el riesgo de no poder agarrar la señal y que se me quede algun hijo zombie. 
    // Ademas en UNIX si estoy atendiendo una señal del mismo tipo que me llega no se acumula sino que 
    // continuo atendiendo la actual (que es la misma que me llego). 
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) { 
        sleep(1);
        for(int i = 0; i < N; i++) {
            if (childs[i] == pid) {
                childs[i] = -1; 
                break;
            }
        }
    }
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
            if(pid != -1){
                printf("Le mando una señal a mi hijo con pid: %d\n", pid);
                kill(pid, SIGTERM); 
            }
        }   
    }

    for(int i = 0; i < N; i++){
        pid = childs[i];
        if (pid != -1){
            sleep(3);
            printf("El proceso con id %d sobrevivio!\n", pid); 
            kill(pid, SIGKILL); 
        }
    }

    return 0; 
}