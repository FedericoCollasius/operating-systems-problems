#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// Parte a) con dos pipes
void a() {
    int fd1[2]; // Pipe del padre al hijo
    int fd2[2]; // Pipe del hijo al padre

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        printf("Error al armar los pipes\n");
        exit(1);
    }

    int i;
    pid_t pid = fork();

    if (pid == 0) { // Hijo
        close(fd1[1]); // Cierro extremo de escritura del padre
        close(fd2[0]); // Cierro extremo de lectura del mio 

        while(1) {
            read(fd1[0], &i, sizeof(int));
            i++;
            printf("HIJO envia a PADRE el valor %d\n", i);
            write(fd2[1], &i, sizeof(int));
            sleep(1);
        }
    } 
    else if (pid > 0) { // Padre
        close(fd1[0]); // Cierro extremo de lectura mio
        close(fd2[1]); // Cerrar extremo de escritura del hijo

        i = 0; 
        printf("PADRE envia a HIJO el valor %d\n", i);
        write(fd1[1], &i, sizeof(int));
        
        while(1) {
            read(fd2[0], &i, sizeof(int));
            i++;
            printf("PADRE envia a HIJO el valor %d\n", i);
            write(fd1[1], &i, sizeof(int));
            sleep(1);
        }
    } 
    else {
        printf("Hubo un error al hacer el fork\n");
        exit(1);
    }
}

// Parte a) con un pipe. 
void a2(){
    int fd[2]; 

    if (pipe(fd) == -1) {
        printf("Error al armar los pipes\n");
        exit(1);
    }

    int i;
    pid_t pid = fork();

    if (pid == 0) { // Hijo
        while(1) {
            read(fd[0], &i, sizeof(int));
            i++;
            printf("HIJO envia a PADRE el valor %d\n", i);
            write(fd[1], &i, sizeof(int));
            sleep(1);
        }
    } 
    else if (pid > 0) { // Padre
        i = 0; 
        printf("PADRE envia a HIJO el valor %d\n", i);
        write(fd[1], &i, sizeof(int));
        sleep(1);
        while(1) {
            read(fd[0], &i, sizeof(int));
            i++;
            printf("PADRE envia a HIJO el valor %d\n", i);
            write(fd[1], &i, sizeof(int));
            sleep(1);
        }
    } 
    else {
        printf("Hubo un error al hacer el fork\n");
        exit(1);
    }

}

void b(){
    int fd1[2]; // Pipe de escritura del Padre y lectura del Hijo_1 
    int fd2[2]; // Pipe de escritura del Hijo_1 y lectura del Hijo_2

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        printf("Error al armar los pipes\n");
        exit(1);
    }

    int i = 0; 
    pid_t pid = fork();

    if (pid == 0) { // Hijo_1
        close(fd1[1]);
        close(fd2[0]);
        while(i < 48) {
            read(fd1[0], &i, sizeof(int));
            i++;
            printf("HIJO_1 envia a HIJO_2 el valor %d\n", i);
            write(fd2[1], &i, sizeof(int));
            sleep(1);
        }
    } 
    else if (pid > 0) { // Creo Hijo_2
        pid_t pid = fork();
        if (pid == 0) {
            close(fd1[0]);
            close(fd1[1]);
            while(i < 48){
                read(fd2[0], &i, sizeof(int));
                i++;
                printf("HIJO_2 envia a PADRE el valor %d\n", i);
                write(fd2[1], &i, sizeof(int));
                sleep(1);
            }
        } else{ // Padre
            close(fd1[0]);
            close(fd2[1]);
            printf("PADRE envia a HIJO_1 el valor %d\n", i);
            write(fd1[1], &i, sizeof(int));
            sleep(1);
            while(i < 48){
                read(fd2[0], &i, sizeof(int));
                i++;
                printf("PADRE envia a HIJO_1 el valor %d\n", i);
                write(fd1[1], &i, sizeof(int));
                sleep(1);
            }
        } 
    } 
}

int main() {
    // a();
    // a2();
    b();
    return 0;
}
