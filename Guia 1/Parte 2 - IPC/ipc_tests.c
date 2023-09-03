#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

// Ver asignacion de indice que hace pipe a los fd.
void test1(){
    int fd[2]; 
    printf("Soy fd[0] antes del pipe: %d\n", fd[0]);
    printf("Soy fd[1] antes del pipe: %d\n", fd[1]);
    pipe(fd);
    printf("Soy fd[0] despues del pipe: %d\n", fd[0]);
    printf("Soy fd[1] despues del pipe: %d\n", fd[1]);
    int fd2[2]; 
    printf("Soy fd2[0] antes del pipe: %d\n", fd2[0]);
    printf("Soy fd2[1] antes del pipe: %d\n", fd2[1]);
    pipe(fd2);
    printf("Soy fd2[0] despues del pipe: %d\n", fd2[0]);
    printf("Soy fd2[1] despues del pipe: %d\n", fd2[1]);
}

// Idem pero con los hijos.
void test2(){
    int fd[2];
    pipe(fd); 
    pid_t pid = fork();
    if(pid == 0){
        printf("Soy el hijo y fd[0] es: %d\n", fd[0]);
        printf("Soy el hijo y fd[1] es: %d\n", fd[1]);
        close(fd[0]);
        close(fd[1]);
    } else {
        sleep(3);
        printf("Soy el padre y fd[0] es: %d\n", fd[0]);
        printf("Soy el padre y fd[1] es: %d\n", fd[1]);
    }
}

// Esto es para ver como el read pisa el valor que habia en el buffer. 
void test3(){
    int fd[2][2];
    pipe(fd[0]);
    pipe(fd[1]);
    pid_t pid; 
    if((pid = fork()) == 0){
        printf("Soy el primer hijo\n");
        printf("Cierro los pipes que no voy a usar\n");
        close(fd[0][0]);
        close(fd[1][0]);
        close(fd[1][1]);
        int a = 1; 
        printf("Escribo en el pipe 0\n");
        write(fd[0][1], &a, sizeof(int));
        printf("Cierro el pipe 0\n");
        close(fd[0][1]);
        printf("Termino el primer hijo\n");
        exit(0);
    } else {
        wait(NULL);
        if((pid = fork()) == 0){
            printf("Soy el segundo hijo\n");
            printf("Cierro los pipes que no voy a usar\n");
            close(fd[0][0]);
            close(fd[0][1]);
            close(fd[1][0]);
            int b = 2; 
            printf("Escribo en el pipe 1\n");
            write(fd[1][1], &b, sizeof(int));
            printf("Cierro el pipe 1\n");
            close(fd[1][1]);
            printf("Termino el segundo hijo\n");
            exit(0);
        } else {
            wait(NULL);
            printf("Soy el padre\n");
            printf("Cierro los pipes que no voy a usar\n");
            close(fd[0][1]);
            close(fd[1][1]);
            int a;
            int res; 
            for(int i = 0; i < 2; i++){
                printf("Leo del pipe %d\n", i);
                read(fd[i][0], &a, sizeof(int));
                res = res + a;
                close(fd[i][0]);
            }
            printf("El resultado es: %d\n", res);
            printf("Termino el padre\n");
        }
    }
}



int main(){
    // test1();
    // test2();
    test3();
    return 0; 
}