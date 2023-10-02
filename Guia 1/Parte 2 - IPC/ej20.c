#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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

void ejecutarHijo (int i, int N, int pipes[][2]) {
    int secreto;
    read(pipes[i][0], &secreto, sizeof(secreto));
    int internalPipe[2];
    pipe(internalPipe);
    int hijo_pid = getpid();
    int nieto_pid = fork();
    if(nieto_pid == 0){
        int res = calcular(secreto);
        write(internalPipe[1], &res, sizeof(res));
    } else {
        wait(NULL);
        int res_nieto; 
        read(internalPipe[0], &res_nieto, sizeof(res_nieto));
        int consulta_padre; 
        read(pipes[i][0], &consulta_padre, sizeof(consulta_padre));
        if(!consulta_padre){
            consulta_padre = 1; 
            write(pipes[N + i][1], &consulta_padre, sizeof(consulta_padre));
            write(pipes[N + i][1], &secreto, sizeof(secreto));
            write(pipes[N + i][1], &res_nieto, sizeof(res_nieto));
        }
    }
    exit(0);
}

int main(int argc, char* argv[]){
    if (argc< 2) {
        printf ("Debe ejecutar con la cantidad de hijos como parametro\n");
        return 0; 
    }

    int N = atoi(argv[1]);
    int pipes[N*2][2];
    for (int i=0; i< N*2; i++){
        pipe(pipes[i]); 
    }
    
    // Primero n son para que el padre haga consultas, siguientes n son para que el hijo responda. 
    for (int i=0; i< N; i++) {
        pid_t pid = fork();
        if (pid==0) {
            ejecutarHijo(i, N, pipes);
            return 0;
        } else {
            int numero = dameNumero(pid);
            write(pipes[i][1], &numero, sizeof(numero)); 
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
            int termino = 0;
            write(pipes[i][1], &termino, sizeof(termino));
            read(pipes[N+i][0], &termino, sizeof(termino));
            if (termino){
                int numero;
                int resultado;
                read(pipes[N+i][0], &numero, sizeof(numero));
                read(pipes[N+i][0], &resultado, sizeof(resultado));
                informarResultado(numero, resultado);
                hijoTermino[i] = 1;
                cantidadTerminados++; 
            } 
        } 
    }
    wait(NULL);
    
    return 0; 
}
