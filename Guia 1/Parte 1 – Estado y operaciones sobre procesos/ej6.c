#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() exec()
#include <sys/wait.h> // wait()


int main(int argc, char const* argv[]){
    const char *command = argv[1]; 
    pid_t pid; 
    if((pid = fork()) == 0){
        printf("Soy el proceso hijo y voy a ejecutar el siguiente comando\n"); 
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        exit(0); 
    }
    wait(NULL); 
    printf("Soy el padre y estoy por terminar\n");
    return 0;
}
