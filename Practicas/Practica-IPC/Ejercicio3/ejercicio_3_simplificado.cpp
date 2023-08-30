#include <sys/wait.h>
#include <unistd.h>
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

void ejecutarHijo(int pipes[][2], int i) {
  long minimo;
  read(pipes[i][0], &minimo, sizeof(minimo));

  long maximo;
  read(pipes[i][0], &maximo, sizeof(maximo));

  long totalPares = contarPares(minimo, maximo);
  write(pipes[i][1], &totalPares, sizeof(totalPares));
}

int main(int argc, char const* argv[]) {
  procesos = atoi(argv[1]);
  int pipes[procesos][2];

  for (int i = 0; i < procesos; ++i) {
    pipe(pipes[i]);
  }

  for (int i = 0; i < procesos; ++i) {
    if (fork() == 0) {
      ejecutarHijo(pipes, i);
      return 0;
    }
  }

  long cantidad = ((RANGO_MAX - RANGO_MIN) + (procesos - 1)) / procesos;
  long inicio_rango = RANGO_MIN;
  for (int i = 0; i < procesos; ++i) {
    long fin_rango = min(inicio_rango + cantidad, RANGO_MAX);
    write(pipes[i][1], &inicio_rango, sizeof(inicio_rango));
    write(pipes[i][1], &fin_rango, sizeof(fin_rango));
    inicio_rango += cantidad;
  }

  for (int i = 0; i < procesos; ++i) {
    wait(NULL);
  }

  long resultado = 0;
  for (int i = 0; i < procesos; ++i) {
    long resultado_i;
    read(pipes[i][0], &resultado_i, sizeof(resultado_i));
    resultado += resultado_i;
  }

  cout << "Resultado total: " << resultado << endl;
  return 0;
}
