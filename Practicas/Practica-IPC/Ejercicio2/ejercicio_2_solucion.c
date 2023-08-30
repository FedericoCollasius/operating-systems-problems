#include <stdio.h>   // printf()
#include <stdlib.h>  // exit()
#include <unistd.h>  // fork() pipe() execlp() dup2() close()

void ejecutar_cmd(char* cmd, char* p) {
  execlp(cmd, cmd, p, NULL);
}

void ejecutar_hijo_1(int pipe_fd[]) {
  // Cerrar lectura
  close(pipe_fd[0]);
  // Conectar escritura a stdout
  dup2(pipe_fd[1], STDOUT_FILENO);
  // Ejecutar programa
  ejecutar_cmd("ls", "-al");
}

void ejecutar_hijo_2(int pipe_fd[]) {
  // Cerrar escritura
  close(pipe_fd[1]);
  // Conectar lectura a stdin
  dup2(pipe_fd[0], STDIN_FILENO);
  // Ejecutar programa
  ejecutar_cmd("wc", "-l");
}

int main(int argc, char const* argv[]) {
  int pipe_fd[2];
  pipe(pipe_fd);

  if (fork() == 0) {
    ejecutar_hijo_1(pipe_fd);
  }

  if (fork() == 0) {
    ejecutar_hijo_2(pipe_fd);
  }

  return 0;
}