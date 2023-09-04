#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PREV(i, n) ((i+n-1) % n)
#define TERMINATE_SIGNAL -1

int main(int argc, char **argv)
{   
    int leer, escribir, pid, child2Wait, n;
    int buffer;

    if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
    /* Parsing de los argumentos */
    n = atoi(argv[1]);
    buffer = atoi(argv[2]);
    int start = atoi(argv[3]);
    printf("Se crearán %i procesos, se enviará el número %i desde proceso %i \n", n, buffer, start);
    
    /* Inicializar semilla aleatoria */
    srand(time(NULL));
  
    /* El padre crea n+1 pipes (el último fuera del anillo)*/
    int ring_pipes[n][2];
    for(int j = 0; j < n; j++) 
        if (pipe(ring_pipes[j]) < 0){ perror("Error al crear el pipe\n"); exit(0);}

    /* El padre crea n hijos que conocen todos los pipes */
    int i;
    for(i = 0; i < n; i++) 
    {
        if((pid = fork()) == 0 ) {
        	if(i == start) child2Wait = pid;
			break;
		}
    }

    /* Si soy el hijo que empieza, escribo el numero inicial */
    if (i == start)
        write(ring_pipes[i][1], &buffer, sizeof(int));

    /* Soy un hijo */
    if(pid == 0)
    {       
        int secret_num = 0;

        if (i == start){
            secret_num = rand() % 20;
            printf("Soy el proceso %d y mi número secreto es %d\n", i, secret_num);
			for(int j = 0; j < n; j++){
				if(j != PREV(start, n)){
					close(ring_pipes[j][0]);
				}
			}
        } else {
			for(int j = 0; j< n; j++){
        		if (j != i)
            		close(ring_pipes[j][1]);
        		if (j != PREV(i,n))     
            		close(ring_pipes[j][0]);                 
    		}			
		}
        
        do 
        {
            read(ring_pipes[PREV(i,n)][0], &buffer, sizeof(int));
            
            if(buffer == TERMINATE_SIGNAL)
                break;
                
            printf("Soy el hijo %d, con numero %d\n", i, buffer);
            
            if (i == start && buffer >= secret_num)
            {
				printf("Soy el hijo que empieza la comunicacion (%d) y superamos al numero secreto\n", i);
				write(ring_pipes[start][1], &buffer, sizeof(int));
                int terminate = TERMINATE_SIGNAL;
				printf("Mando la señal de terminacion a los hijos\n");
                for(int j=0; j < n; j++){
					if (j != i)
                    	write(ring_pipes[PREV(j, n)][1], &terminate, sizeof(int));
						close(ring_pipes[PREV(j, n)][1]);
				}
                break;
            }
            
            buffer++;
            
            write(ring_pipes[i][1], &buffer, sizeof(int));
        } while (1);
        
		close(ring_pipes[PREV(i, n)][0]);
		close(ring_pipes[i][1]);
		printf("Soy el hijo %d y termine.\n", i);
        exit(0);
    }

    /* El padre cierra pipes, espera al ultimo hijo y lee el ultimo valor*/
    else if(pid > 0)
    {
        for(int j = 0; j < n; j++)
        {
            close(ring_pipes[j][1]); 
            if(j != start)
                close(ring_pipes[j][0]);         
        }
		
        waitpid(child2Wait, NULL,0);
        
        read(ring_pipes[start][0], &buffer, sizeof(int));
		close(ring_pipes[start][0]);
        printf("Soy el proceso PADRE, y el numero final es %d\n", buffer);

        exit(0);
    }
}
