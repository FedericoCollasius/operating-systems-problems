#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()

int main(int argc, char const *argv[]){
    int dato = 0;
    pid_t pid = fork();
    //si no hay error, pid vale 0 para el hijo
    //y el valor del process id del hijo para el padre
    if (pid == -1) exit(EXIT_FAILURE);
    //si es -1, hubo un error
    else if (pid == 0) {
        for (int i=0; i< 3; i++) {
            dato++;
            printf("Dato hijo: %d\n", dato);
        }
    }
    else {
        for (int i=0; i< 3; i++) {
            dato++;
            printf("Dato padre: %d\n", dato);
        }
    }
    exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}

/*
Los resultados no son iguales: 
Dato padre: 0
Dato padre: 0
Dato padre: 0
Dato hijo: 1
Dato hijo: 2
Dato hijo: 3

Como el padre no aumenta su variable en su contexto
dato queda igual que como se inicializo. Si aumento 
dato en el contexto del padre vamos a tener esto: 

Dato padre: 1
Dato padre: 2
Dato padre: 3
Dato hijo: 1
Dato hijo: 2
Dato hijo: 3
*/