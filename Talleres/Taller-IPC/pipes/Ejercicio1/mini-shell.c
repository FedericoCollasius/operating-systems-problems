#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

static int run(char ***progs, size_t count)
{
    int r, status;

    // Reservo memoria para el arreglo de pids
    pid_t *children = malloc(sizeof(*children) * count);

    // Creo los pipes. Si hay count programas, hay count - 1 pipes. 
    int pipes[count - 1][2];
    for (int i = 0; i < count - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
    }

    // Creo los procesos hijos
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (pid == 0) {
            // Hijos

            // Si el proceso hijo NO es el primero, redirecciono stdin (su entrada estandar)
			// al extremo de lectura del pipe anterior. 
            if (i > 0) {
				// Conectar lectura a stdin
				dup2(pipes[i - 1][0], STDIN_FILENO);
            }
			
			// Si el proceso hijo NO es el ultimo, redirecciono stdout (su salida estandar)
			// al extremo de escritura del pipe anterior. 
            if (i < count - 1) {
				// Conectar escritura a stdout
			    dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Cierro todos los extremos de los pipes.  
            for (int j = 0; j < count - 1; j++) {
        		close(pipes[j][0]);
        		close(pipes[j][1]);
    		}

            // Ejecuto el programa
            execvp(progs[i][0], progs[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
		// Padre
        children[i] = pid;
    }

    // Cierro todos los extremos de los pipes en el proceso padre.
    for (int i = 0; i < count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Espero a los hijos y verifico el estado en que terminaron
    for (int i = 0; i < count; i++) {
        waitpid(children[i], &status, 0);

        if (!WIFEXITED(status)) {
            fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
                (int)children[i], WIFSIGNALED(status));
            perror("");
            return -1;
        }
    }
    r = 0;
    free(children);

    return r;
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n");
        printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
        return 0;
    }
    int programs_count;
    char*** programs_with_parameters = parse_input(argv, &programs_count);

    printf("status: %d\n", run(programs_with_parameters, programs_count));

    fflush(stdout);
    fflush(stderr);

    return 0;
}
