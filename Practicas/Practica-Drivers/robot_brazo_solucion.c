/*

% SOLUCION:
- lector: 
	+ operaciones:
		* read: devuelve el código de barras leído
	+ registros:
		* CODIGO: codigo de barras

	read(char* buffer){
		int codigo = IN(CODIGO)
		copy_to_user(buffer, &codigo, sizeof(codigo));
		return sizeof(codigo);
	}

- brazo: 
	+ operaciones:
		* write: indica al brazo hacia dónde moverse, y qué hacer con la mano
	+ registros:
		* POSICION: 0, 1,...,10, 0 para el robot, 1 a 10 para la estantería, 11 para la cinta
		* MANO: ABRIR, CERRAR: indica qué debe hacer cuando llegue a la posición

	write(char* buffer){
		brazo_operacion p;
		copy_from_user(buffer, &p, sizeof(p));
		OUT(POSICION, p.posicion);
		OUT(MANO, p.mano);
		return 1;
	}

- movimiento:
	+ operaciones:
		* write: indica que debe moverse, retorna cuando haya llegado a destino
	+ registros:
		* DEST_X: indica la posición X
		* DEST_Y: indica la posición Y
		* CONTROL: START
	+ interrupciones: se levanta la interrupción INT_MOV_OK cuando se llegue a destino.

	sem sem = sem_init(0);

	handler() {
		sem.signal();
	}

	init() {
		map_irq(INT_MOV_OK, handler)
	}

	write(char* buffer) {
		ubicacion u;
		copy_from_user(buffer, &ubicacion, sizeof(ubicacion));

		OUT(DEST_X, u.x);
		OUT(DEST_Y, u.y);
		OUT(CONTROL, START);

		sem.wait();

		return sizeof(ubicacion);
	}
*/

struct paquete {
	int x;
	int y;
	int codigo;
}

struct brazo_operacion {
	int posicion;
	enum mano {ABRIR, CERRAR};
}

struct ubicacion {
	int x;
	int y;
}

void tomar_objeto(int i) {
	op = {posicion=i, mano=CERRAR};
	write(brazo, op, sizeof(op));
}

void main() {
	int lector = open("/dev/lector");
	int brazo = open("/dev/brazo");
	int movimiento = open("/dev/movimiento");
	brazo_operacion op;

	while(1) {
		paquete p = siguiente_paquete();
		ubicacion u = {x=p.x, y=p.y};
		write(movimiento, u, sizeof(u)); // se bloquea hasta que llegue

		// llegó a la estantería
		for(int i=1; i<=10; i++) {
			// Tomar el objeto de la estantería
			tomar_objeto(i);

			// Acercarlo al robot y leerlo
			tomar_objeto(0);

			int codigo;
			read(lector, &codigo, sizeof(codigo));
			if(codigo==p.codigo) {
				break;
			}

			// Devolver el objeto a la estantería
			soltar_objeto(i);
		}

		// Al salir del for, el objeto está en la mano del robot
		// Moverse hasta la cinta de entrega, y soltar el paquete
		ubicacion cinta = {x=0, y=0};
		write(movimiento, cinta, sizeof(p)); // se bloquea hasta que llegue
		op = {posicion=11, mano=ABRIR};
		write(brazo, op, sizeof(op));
	}
}
