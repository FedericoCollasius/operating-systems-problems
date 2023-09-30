# Consigna

Modelar con semaforos un micro de larga distancia directo entre Buenos Aires y Mendoza. El micro tiene capacidad para N personas y funciona de la siguiente manera:

- Empieza en Buenos Aires.
- Espera a llenarse.
- Viaja hasta Mendoza.
- Estaciona en una terminal, permitiendo que los pasajeros desciendan.
- Repite el procedimiento desde el principio pero desde la otra terminal.

Notar que para subir al micro no importa el orden de llegada. Ademas el micro permite que pasajeros puedan subir y bajar al mismo tiempo.

# Mi solucion

sem permisoSubir[2] = {sem(0), sem(0)}
sem permisoBajar(0)
int esperando[2] = {0, 0}
sem mutexEsperando(1)
sem MutexMicro(0)

enum estacion = {BUENOS AIRES = 0, MENDOZA = 1}

Bus() {

    int estacionActual = BUENOS AIRES
    while(True){
        while(esperando[estacionActual] < N){
            mutexMicro.wait()
        }
        for(int i = 0; i < N; i++){
            esperando[estacionActual]--
            permisoSubir[estacionActual].signal()
        }

        // Viajar
        if (estacionActual == BUENOS AIRES) {
            BuenosAiresAMendoza()
            estacionActual = MENDOZA
        } else {
            MendozaABuenosAires()
            estacionActual = BUENOS AIRES
        }

        // Permitir a los pasajeros bajar
        for(int i = 0; i < N; i++){
            permisoBajar.signal()
            mutexMicro.wait()
        }
    }

}

Pasajeros(estacion e) {

    mutexEsperando.wait()
    esperando[e]++
    mutexEsperando.signal()
    mutexMicro.signal()
    permisoSubir[e].wait()
    permisoBajar.wait()
    Bajar()
    mutexMicro.signal()

}

# Solucion de la Catedra

permisoSubir = [sem(0), sem(0)]
asientos = sem(N)
permisoBajar = sem(0)
mutex = sem(1)
permisoViajar = sem(0)
asientosOcupados = 0

bus(terminal):

    while true:
    repeat N times:
    permisoSubir[terminal].signal()
    permisoViajar.wait()
    viajar()
    repeat N times:
    permisoBajar.signal()
    terminal = 1 - terminal

pasajero(i, terminal):

    permisoSubir[terminal].wait()
    asientos.wait()
    subir()
    mutex.wait()
    asientosOcupados++
    if (asientosOcupados == N):
    permisoViajar.signal()
    mutex.signal()
    permisoBajar.wait()
    bajar(i, terminal)
    asientos.signal()
