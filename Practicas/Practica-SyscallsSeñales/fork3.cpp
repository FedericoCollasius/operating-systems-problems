#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void julieta() {
	write(1,"Soy Julieta\n", 12);
	pid_t pid = fork();
	if (pid == -1) {
			exit(EXIT_FAILURE);
	} else { 
		if (pid == 0) { 
			write(1, "Soy Jennifer\n", 14);
			sleep(1);
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
	}
}

void juan() {
	write(1, "Soy Juan\n", 10);
	wait(NULL);
  	pid_t pid = fork();
    if (pid == -1){ 
		exit(EXIT_FAILURE);
	} else { 
		if (pid == 0) {
			write(1, "Soy Jorge\n", 10);
			sleep(1);						
		}
	}	
}

int main(int argc, char const *argv[]){
  pid_t pid = fork();
  //si no hay error, pid vale 0 para el hijo
  //y el valor del process id del hijo para el padre
  if (pid == -1) { 
	exit(EXIT_FAILURE);
  } else if (pid == 0) {
	julieta();	
  } else {
	juan();
  }
  exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}