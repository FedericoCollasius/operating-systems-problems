#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void test_1(){
    printf("Soy el padre\n");
    pid_t child = fork(); 
    if(child == 0){
        printf("Soy el hijo\n");
        exit(0);
    } else {
        int status;
        wait(&status);
        printf("Soy el padre y termine mi hijo termino con status: %d\n", WEXITSTATUS(status));
    }
}

void test_2() {
    printf("Soy el padre\n");
    pid_t child = fork(); 

    if(child == 0) { // Proceso hijo
        printf("PID hijo: %d\n", getpid());
        pid_t child_2 = fork();

        if(child_2 == 0) { // Proceso nieto
            printf("PID nieto: %d\n", getpid());
            while(1); // Bucle infinito para el nieto
        } else {
            int status_2;
            waitpid(child_2, &status_2, 0); // El hijo espera al nieto
            printf("El nieto ha terminado. PID hijo: %d\n", getpid());
            exit(0); // El hijo termina después de que el nieto haya terminado
        }
    } else {
        int status;
        waitpid(child, &status, 0); // El padre espera al hijo
        printf("Soy el padre y mi hijo terminó con estado: %d\n", WEXITSTATUS(status));
    }
}

int main() {
    return 0; 
}






