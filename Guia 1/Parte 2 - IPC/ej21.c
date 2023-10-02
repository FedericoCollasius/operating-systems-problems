#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() close()
#include <sys/types.h> // pid_t 
#include <signal.h> // SIGINT

int pipe_fd[2];
pid_t hijo; 

void handler_sigint_hijo(int signum){
   printf("Dejame pensarlo...\n");
   sleep(5);
   printf("Ya se el significado de la vida.\n");
   write(pipe_fd[1], "42", 2);
   kill(getppid(), SIGINT); 
}

void handler_sighup_hijo(int signum){
    printf("Me voy a mirar crecer las flores.\n");
    exit(0);
}

void handler_sigint_padre(int signum){
    char buf[3];
    read(pipe_fd[0], buf, 2);
    printf("Mirá vos. El significado de la vida es %s\n", buf);
    printf("¡Bang Bang, estás liquidado!\n");
    kill(hijo, SIGHUP);
    sleep(10);
}

void handler_sigchld_padre(int signum){
    printf("Te voy a buscar en la oscuridad.\n");
    exit(0);
}

int main(){
    pipe(pipe_fd);
    hijo = fork();

    if(hijo == 0){
        signal(SIGINT, handler_sigint_hijo);
        signal(SIGHUP, handler_sighup_hijo);
        while(1);
    }

    sleep(1);
    printf("¿Cual es el significado de la vida?\n");
    kill(hijo, SIGINT);
    signal(SIGINT, handler_sigint_padre);
    signal(SIGCHLD, handler_sigchld_padre);

    while(1);

    exit(0);
}