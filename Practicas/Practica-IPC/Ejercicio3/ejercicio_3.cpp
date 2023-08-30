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

int main(int argc, char const* argv[]) {
  // Contar el tiempo de ejecución del programa
  auto start = chrono::steady_clock::now();

  // Parseamos los parámetros
  if (argc != 2) {
    cout << "Debe ejecutar con la cantidad de procesos N como parámetro. "
            "Ejemplo:"
         << endl;
    cout << argv[0] << " N" << endl;
    return 1;
  }
  procesos = atoi(argv[1]);

  // Creamos los N pipes

  // Creamos los N hijos

  // Calculamos e informamos los intervalos

  // Recolectamos resultados
  long resultado = 0;

  // Imprimimos resultado total
  cout << "Resultado total: " << resultado << endl;

  auto end = chrono::steady_clock::now();
  cout << "Tiempo total (ms): "
       << chrono::duration_cast<chrono::milliseconds>(end - start).count()
       << " ms" << endl;

  return 0;
}
