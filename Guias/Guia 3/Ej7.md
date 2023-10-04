# Consigna

Se tienen N procesos, P0,P1, ..., PN−1 (donde N es un parámetro).
Se requiere sincronizarlos de manera que la secuencia de ejecución sea Pi,Pi+1, ..., PN−1,P0, ..., Pi−1 (donde i es otro parámetro).

Escriba el código que deben ejecutar cada uno de los procesos para cumplir con la sincronización
requerida utilizando semáforos (no olvidar los valores iniciales).

# Solucion

sem mutex(1);
sem avanzarProceso[N](0);

P(k, i, N){

    mutex.lock();
    if (k == i) {
        avanzarProceso[(k + 1) % N].signal();
    }
    mutex.unlock();

    if(k != i){
        avanzarProceso[k].wait();
        avanzarProceso[(k + 1) % N].signal()
    }

}
