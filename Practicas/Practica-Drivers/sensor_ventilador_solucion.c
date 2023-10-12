/*

% SOLUCION:
- sensor: 
	+ operaciones:
		* read: devuelve el promedio de los ultimos T segundos
		* write: Guardamos el valor de T
	+ registros:
		* TEMP: Promedio de los ultimos T segundos
		* TEMP_MAX: 
		* TEMP_MIN:
		* T: T segundos
	+ interrupciones: se levanta la interrupción INT_MOV_OK cuando se llegue a destino.

	read(char* buffer){
		sem.wait();
		int temp = IN(TEMP)
		copy_to_user(buffer, &temp, sizeof(temp));
		return sizeof(temp);
	}

	write(char* buffer){
		sensor_config config;
		copy_from_user(buffer, &p, sizeof(p));
		OUT(T, config.t);
		OUT(TEMP_MAX, config.temp_max)
		OUT(TEMP_MIN, config.temp_min)
		return 3*sizeof(config.t);
	}

	init() {
		map_irq(INT_MOV_OK, handler)
	}

	handler() {
		sem.signal();
	}

- ventilador: 
	+ operaciones:
		* write: Configura maximo y minimo
	+ registros:
		* ENCENDIDO: on/off
		 

	write(char* buffer){
		bool p;
		copy_from_user(buffer, &p, sizeof(p));
		OUT(ENCENDIDO, p;
		return 1;
	}
*/

struct sensor_config {
	int T;
	int temp_max;
	int temp_min;

}

int OFF = 0
int ON = 1

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