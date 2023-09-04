#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

#define RANGO_MIN 2L
#define RANGO_MAX 1000000001L

using namespace std;

int procesos;

bool esPar(long numero) {
  return (numero & 1) == 0;
}

long contarPares(long minimo, long maximo) {
  long cantidad = 0;
  for (long i = minimo; i < maximo; ++i) {
    if (esPar(i)) {
      cantidad++;
    }
  }
  return cantidad;
}

// Deberia pasarle todos los fds? 
void hijo(int i, int fds[][2]){
  // Cierra todos los descriptores de archivo, excepto los del proceso hijo actual
  for(int j = 0; j < procesos; j++) {
    if(j != i) {
      close(fds[j][0]);
      close(fds[j][1]);
    }
  }
  long minimo; 
  read(fds[i][0], &minimo, sizeof(minimo)); 
  long maximo; 
  read(fds[i][0], &maximo, sizeof(maximo));

  long cantidad = contarPares(minimo, maximo);
  write(fds[i][1], &cantidad, sizeof(cantidad));

  close(fds[i][0]); 
  close(fds[i][1]);
}

int main(int argc, char const* argv[]) {
  // Contar el tiempo de ejecución del programa
  auto start = chrono::steady_clock::now();

  // Parseamos los parámetros
  if (argc != 2) {
    std::cout << "Debe ejecutar con la cantidad de procesos N como parámetro. Ejemplo:" << std::endl;
    std::cout << argv[0] << " N" << std::endl;
    return 1;
  }
  procesos = atoi(argv[1]);

  // Creamos los N pipes
  int fds[procesos][2]; 
  for(int i = 0; i < procesos; i++){
    pipe(fds[i]);
  }

  // Creamos los N hijos
  pid_t pid;
  int i = 0; 
  for(int i = 0; i < procesos; i++){
    if((pid = fork()) == 0){
      hijo(i, fds);
      exit(0);
    }
  }

  // Calcular e informar rangos
  long cantidad = ((RANGO_MAX - RANGO_MIN) + (procesos - 1)) / procesos;
  long inicio_rango = RANGO_MIN;
  for (int i = 0; i < procesos; ++i) {
    long fin_rango = min(inicio_rango + cantidad, RANGO_MAX);
    write(fds[i][1], &inicio_rango, sizeof(inicio_rango));
    write(fds[i][1], &fin_rango, sizeof(fin_rango));
    close(fds[i][1]);
    inicio_rango += cantidad;
  }

  // Esperamos a los hijos
  for(int i = 0; i < procesos; i++){
    wait(NULL);
  }

  // Recolectamos resultados
  long resultado = 0;
  long buffer; 
  for(int i = 0; i < procesos; i++){
    read(fds[i][0], &buffer, sizeof(buffer));
    resultado = resultado + buffer;
    close(fds[i][0]);
  }

  // Imprimimos resultado total
  std::cout << "Resultado total: " << resultado << std::endl;

  auto end = chrono::steady_clock::now();
  std::cout << "Tiempo total (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << std::endl;

  return 0;
}
