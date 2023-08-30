#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() exec() dup2() close()

/**
 *  Debe ejecutar "ls -al"
 */
void ejecutar_hijo_1(int pipe_fd[]) {
  // Completar...
}

/**
 *  Debe ejecutar "wc -l"
 */
void ejecutar_hijo_2(int pipe_fd[]) {
  // Completar...
}

int main(int argc, char const* argv[]) {
  // Completar...

  return 0;
}