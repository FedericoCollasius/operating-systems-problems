Suponer una secuencia de referencias a páginas que contiene repeticiones de largas secuencias de
referencias a páginas seguidas ocasionalmente por una referencia a una página aleatoria. Por ejemplo,
la secuencia: 0, 1, ... , 511, 431, 0, 1, ... , 511, 332, 0, 1, ... consiste en repeticiones de la secuencia 0,1, ... , 511 seguidas por referencias aleatorias a las páginas 431 y 332.

- ¿Por qué los algoritmos de reemplazo LRU, FIFO y Second Chance no serán efectivos para
  manejar esta dinámica para una cantidad de frames menor al largo de la secuencia?

Suponiendo que la pagina random se encuentra siempre en el rango 0-511, siempre que tenga menos
de 512 marcos de pagina tanto FIFO como LRU y SC se van a comportar de la misma manera.

Si tengo una cantidad de marcos x < 512, con cualquiera de esos 3 algoritmos de reemplazo, voy a
poder cargar hasta x paginas. Al solicitarse la pagina x+1, tendre que desalojar la pagina ubicada en
el marco 0. Para cargar la pagina x+2, desalojo la pagina en el marco 1. Y ası sucesivamente. Esto
significa que al volver a solicitar la pagina 0 (y subsiguientes) la misma no estara cargada y
ocasionara un page-fault. La unica chance de no tener page-fault sera que la pagina random se
encuentre entre las ultimas x paginas cargadas. Todo lo demas fallara.

- Si este programa tuviera disponibles 500 frames, describir un enfoque de reemplazo de páginas
  que funcione mejor que los algoritmos LRU, FIFO o Second Chance.

Si tuviera disponibles 500 frames, los algoritmos FIFO, LRU y SC no serıan efectivos por lo descrito
en el item anterior: cargarıa las paginas 0 a 499, y para cargar de la 500 a la 511 deber´ıa desalojar los primeros 12 marcos, ocasionando la misma reaccion en cadena que se explico arriba. A lo sumo,
podre acertar la pagina random.

Una mejor opcion podrıa ser dejar 499 paginas fijas en 499 marcos, y utilizar el marco restante para
cargar las demas, de a una a la vez. Ası, tendrıa 499 page-faults a la primera, 12 o 13 page-faults mas
para las ultimas (segun si la random esta entre las fijas o no), y a partir de este punto las 499 paginas
fijas ya no fallaran. Las ´ultimas 12 o 13 sı, pero reduce muchısimo el page-fault rate.

Otra alternativa que surgio en clase es, para las 12 o 13 paginas finales de cada tanda, desalojar
paginas en un orden aleatorio. La intuicion detras de esta idea es que nunca podre tener menos de 12
page-faults, y si voy desalojando paginas con una distribucion medianamente uniforme, puedo lograr
evitar un impacto en la percepcion del usuario, evitando este mini-thrashing que se observa en la idea
anterior.
