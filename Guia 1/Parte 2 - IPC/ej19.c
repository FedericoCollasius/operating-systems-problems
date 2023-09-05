#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() execlp() dup2() close()
#include <sys/types.h> // pid_t getpid() getppid()
#include <stddef.h>  // size_t

int calcular_promedio(const int *lista, size_t tam) {
    if (lista == NULL || tam == 0) {
        return 0;
    }

    int suma = 0;
    for (size_t i = 0; i < tam; i++) {
        suma += lista[i];
    }

    return suma / tam;
}

int main(){
    return 0; 
}