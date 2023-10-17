# Consigna: sistema de refrigreacion.

## Compuesto por:

- Un ventilador.
- Un sensor de temperatura ambiente con cronometro incorporado.

Cada dispositivo debe ser manejado por un driver independiente.

## Funcionamiento:

- Al iniciarse, la aplicacion de usuario debera recibir tres parametros de configuracion por entrada estandar: un umbral de temperatura minima, un umbral de temperatura maxima, y un tiempo T (todos enteros).

### Sensor

- El sensor de temperatura debera poder informar con un numero entero el promedio de la temperatura de los ́ultimos T segundos.
  - Si la temperatura promedio de los ́ultimos T segundos se encuentra por debajo del valor minimo, el ventilador deber apagarse.
  - Si la temperatura promedio de los ́ultimos T segundos se encuentra por arriba del valor maximo, el ventilador debera encenderse.
- Cualquier temperatura que se encuentre entre la minima y la maxima se debera considerar dentro del rango normal de refrigeracion, y no debera tener ningun impacto en el estado del ventilador.

### Ventilador

- Para reducir el consumo energetico del sistema, el ventilador solamente debera cambiar de estado cuando la temperatura promedio se encuentre fuera del rango normal (menor a la temperatura minima o mayor a la maxima).

## Observaciones:

- No esta permitido realizar sleep u otras operaciones similares.

# Solucion

## Diseño

**Sensor**

- Operaciones:
  - read: devuelve el promedio de los ultimos T segundos
  - write: Guardamos el valor de T
- Registros:
  - TEMP: Promedio de los ultimos T segundos
  - TEMP_MAX
  - TEMP_MIN
  - T
- Interrupciones: se levanta la interrupción INT\_ cuando se llegue a destino.

**Ventilador**

- Operaciones:
  - write: Configura maximo y minimo
- Registros:
  - ENCENDIDO: ON,OFF.

## Codigo

```C

// Sensor
void driver_sensor_init(){
    sem_init(sem, 0)
	request_irq(INT_T, handler)
}

handler() {
	sem.signal();
}

int driver_sensor_read(char* buffer){
	sem.wait();
	int temp = IN(TEMP)
	copy_to_user(buffer, &temp, sizeof(temp));
	return sizeof(temp);
}

int driver_sensor_write(char* buffer){
	sensor_config config;
	copy_from_user(buffer, &p, sizeof(p));
	OUT(T, config.t);
	OUT(TEMP_MAX, config.temp_max)
	OUT(TEMP_MIN, config.temp_min)
	return 3*sizeof(config.t);
}

void driver_sensor_exit(){
    sem_destroy(sem)
    free_irq(INT_T)
}

// Ventilador
int driver_ventilador_write(char* buffer){
	bool p;
	copy_from_user(buffer, &p, sizeof(p));
	OUT(ENCENDIDO, p);
	return 1;
}

// Estructuras auxiliares

struct sensor_config {
	int T;
	int temp_max;
	int temp_min;
}

int OFF = 0
int ON = 1

// Sistema de refrigeracion
void main() {
	int ventilador = open("/dev/ventilador");
	int sensor = open("/dev/sensor");

	struct sensor_config;

	scanf(&sensor_config.T)
	scanf(&sensor_config.temp_max)
	scanf(&sensor_config.temp_min)

	//Configuramos el sensor
	write(sensor, sensor_config, sizeof(sensor_config));
	while(1){
		int temperatura;
		read(sensor, &temperatura, sizeof(temperatura));
		if(temperatura < sensor_config.temp_min){
			write(ventilador, OFF, sizeof(OFF));
		} else if (temperatura > sensor_config.temp_max){
			write(ventilador, ON, sizeof(OFF));
		} else {
			continue;
		}
	}
}
```
