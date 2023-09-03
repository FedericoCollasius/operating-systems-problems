#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

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


int main(){
    // test1();
    test2();
    return 0; 
}