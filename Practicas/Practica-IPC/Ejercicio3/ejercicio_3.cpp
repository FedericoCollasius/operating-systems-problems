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

void hijo(int fd[]){
  long minimo; 
  std::cout << "Leo el minimo" << std::endl;
  read(fd[0], &minimo, sizeof(minimo)); 
  long maximo; 
  std::cout << "Leo el maximo" << std::endl;
  read(fd[0], &maximo, sizeof(maximo));
  std::cout << "Cuento la cantidad de pares entre " << minimo << " y " << maximo << std::endl;
  long cantidad = contarPares(minimo, maximo);
  std::cout << "Escribo la cantidad de pares: " << cantidad << std::endl;
  write(fd[1], &cantidad, sizeof(cantidad));
  std::cout << "Cierro los pipes" << std::endl;
  close(fd[0]); 
  close(fd[1]);
  std::cout << "Soy el hijo " << getpid() << " y termine el calculo" << std::endl;
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
      std::cout << "Soy el hijo " << getpid() << std::endl;
      hijo(fds[i]);
      std::cout << "Soy el hijo " << getpid() << " y termine" << std::endl;
      exit(0);
    }
  }

  // Calcular e informar rangos
  std::cout << "Soy el padre y voy a calcular los rangos" << std::endl;
  long cantidad = ((RANGO_MAX - RANGO_MIN) + (procesos - 1)) / procesos;
  long inicio_rango = RANGO_MIN;
  for (int i = 0; i < procesos; ++i) {
    std::cout << "Calculo el rango del hijo " << i << std::endl;
    long fin_rango = min(inicio_rango + cantidad, RANGO_MAX);
    std::cout << "Escribo el rango del hijo " << i << ": " << inicio_rango << " - " << fin_rango << std::endl;
    write(fds[i][1], &inicio_rango, sizeof(inicio_rango));
    write(fds[i][1], &fin_rango, sizeof(fin_rango));
    std::cout << "Cierro el pipe de escritura del hijo " << i << std::endl;
    close(fds[i][1]);
    inicio_rango += cantidad;
  }

  // Esperamos a los hijos
  std::cout << "Soy el padre y voy a esperar a los hijos" << std::endl;
  for(int i = 0; i < procesos; i++){
    wait(NULL);
  }

  // Recolectamos resultados
  std::cout << "Soy el padre y voy a calcular el resultado" << std::endl;
  long resultado = 0;
  long buffer; 
  for(int i = 0; i < procesos; i++){
    std::cout << "Leo el resultado del hijo " << i << std::endl;
    read(fds[i][0], &buffer, sizeof(buffer));
    std::cout << "El resultado del hijo " << i << " es " << buffer << std::endl;
    resultado = resultado + buffer;
    std::cout << "Cierro el pipe de lectura del hijo " << i << std::endl;
    close(fds[i][0]);
  }

  // Imprimimos resultado total
  std::cout << "Resultado total: " << resultado << std::endl;

  auto end = chrono::steady_clock::now();
  std::cout << "Tiempo total (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << std::endl;

  return 0;
}
