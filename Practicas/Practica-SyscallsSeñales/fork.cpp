#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void Subrutina_proceso_hijo(){
	for (int i = 0; i < 3; ++i){
	   sleep(1);
	   write(1,"Soy el proceso hijo\n",20);
	}
}

void Subrutina_proceso_padre(){
	for (int i = 0; i < 3; ++i){
	   sleep(1);
	   write(1,"Soy el proceso padre\n",21);
	}
}

int main(int argc, char const *argv[]){
  pid_t pid = fork();
  //si no hay error, pid vale 0 para el hijo
  //y el valor del process id del hijo para el padre
  if (pid == -1) exit(EXIT_FAILURE);  
  //si es -1, hubo un error
  else if (pid == 0) {
     Subrutina_proceso_hijo();
  }
  else {
     Subrutina_proceso_padre();
  }
  exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}
