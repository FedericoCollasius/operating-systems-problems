#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // fork() 
#include <sys/wait.h> // wait()
#include <signal.h> // signals

// Son 4 procesos. 

int main(){
    pid_t pid; 
    pid = fork(); 
    if(pid == 0){
        printf("Soy Julieta\n");
        sleep(1);
        pid = fork();
        if(pid == 0){
            printf("Soy Jennifer\n");
            sleep(1);
            exit(0);
        } else {
            exit(0); 
        }
    }
    else{
        printf("Soy Juan\n");
        sleep(1);
        wait(NULL);
        pid = fork();
        if(pid == 0){
            printf("Soy Jorge\n");
            sleep(1);
            exit(0);
        } else {
            exit(0);
        }
    }
}