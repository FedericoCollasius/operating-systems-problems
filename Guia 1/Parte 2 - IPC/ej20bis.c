#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int dameNumero(pid_t pid){
    return pid; 
}

int calcular(int numero){
    return numero + 1; 
}

void informarResultado(int numero, int resultado){
    printf("El numero sobre el que se ejecuto el calculo fue %d y el resultado %d \n", numero, resultado);
}

void ejecutarHijo (int i, int pipes[][2]) {
    int secreto;
    read(pipes[i][0], &secreto, sizeof(secreto));
    close(pipes[i][0]);
    int internalPipe[2];
    pipe(internalPipe);
    int hijo_pid = getpid();
    int nieto_pid = fork();
    if(nieto_pid == 0){
        close(internalPipe[0]);
        int res = calcular(secreto);
        write(internalPipe[1], &res, sizeof(res));
        exit(0);
    } else {
        close(internalPipe[1]);
        wait(NULL);
        int res_nieto;
        read(internalPipe[0], &res_nieto, sizeof(res_nieto));
        int termino = 1;
        write(pipes[i][1], &termino, sizeof(termino));
        write(pipes[i][1], &secreto, sizeof(secreto));
        write(pipes[i][1], &res_nieto, sizeof(res_nieto));
        }
}

int main(int argc, char* argv[]){
    if (argc< 2) {
        printf ("Debe ejecutar con la cantidad de hijos como parametro\n");
        return 0; 
    }

    int N = atoi(argv[1]);
    int pipes[N][2];
    for (int i=0; i< N; i++){
        pipe(pipes[i]); 
    }
    
    // Primero n son para que el padre haga consultas, siguientes n son para que el hijo responda. 
    for (int i=0; i< N; i++) {
        pid_t pid = fork();
        if (pid==0) {
            ejecutarHijo(i,pipes);
            return 0;
        } else {
            int numero = dameNumero(pid);
            write(pipes[i][1], &numero, sizeof(numero)); 
            close(pipes[i][1]);
        } 
    }

    int cantidadTerminados = 0;
    int hijoTermino[N]; 
    for(int i = 0; i < N; i++){
        hijoTermino[i] = 0; 
    }
    while (cantidadTerminados < N) {
        for (int i=0; i< N; i++) {
            if (hijoTermino[i]) {
                continue; 
            }
            int termino;
            read(pipes[i][0], &termino, sizeof(termino));
            if (termino){
                int numero;
                int resultado;
                read(pipes[i][0], &numero, sizeof(numero));
                read(pipes[i][0], &resultado, sizeof(resultado));
                informarResultado(numero, resultado);
                hijoTermino[i] = 1;
                cantidadTerminados++; 
            } 
        } 
    }
    wait(NULL);
    
    return 0; 
}
