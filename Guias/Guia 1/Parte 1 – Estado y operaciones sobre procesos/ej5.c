#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    printf("Soy Abraham\n");
    pid_t child = fork(); 
    if(child == 0){
        printf("Soy Homer\n");
        pid_t child_1 = fork(); 
        if (child_1 == 0){
            printf("Soy Bart\n"); 
            exit(0);
        } else {
            wait(NULL); 
        }

        pid_t child_2 = fork();
        if (child_2 == 0){
            printf("Soy Lisa\n"); 
            exit(0);
        } else {
            wait(NULL); 
        }

        pid_t child_3 = fork();
        if (child_3 == 0){
            printf("Soy Maggie\n"); 
            exit(0);
        } else {
            wait(NULL); 
        }
        printf("Soy Homer y termine\n");
        exit(0);
    } else {
        wait(NULL); 
        printf("Soy Abraham y termine\n");
    }
    return 0; 
}


