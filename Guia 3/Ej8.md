# Consigna

Considere cada uno de los siguientes enunciados, para cada caso, escriba el código que permita
la ejecución de los procesos según la forma de sincronización planteada utilizando semáforos (no se
olvide de los valores iniciales). Debe argumentar porqué cada solución evita la inanición:

1. Se tienen tres procesos (A, B y C). Se desea que el orden en que se ejecutan sea el orden
   alfabético, es decir que las secuencias normales deben ser: ABC, ABC, ABC, ...

## Solucion 1

permisoA = sem(1)
permisoB = sem(0)
permisoC = sem(0)

A(){

    while(True){
        permisoA.wait()
        print(A)
        permisoB.signal()
    }

}

B(){

    while(True){
        permisoB.wait()
        print(B)
        permisoC.signal()
    }

}

C(){

    while(True){
        permisoC.wait()
        print(C)
        permisoA.signal()
    }

}

2. Idem anterior, pero se desea que la secuencia normal sea: BBCA, BBCA, BBCA, ...

## Solucion 2

permisoA = sem(0)
permisoB = sem(1)
permisoC = sem(0)

countB = 0

A(){

    while(True){
        permisoA.wait()
        print(A)
        permisoB.signal()
    }

}

B(){

    while(True){
        permisoB.wait()
        print(B)
        countB++
        if(countB == 1){
            permisoB.signal()
        } else{
            countB == 0;
            permisoC.signal()
        }
    }

}

C(){

    while(True){
        permisoC.wait()
        print(C)
        permisoA.signal()
    }

}

3. Se tienen un productor (A) y dos consumidores (B y C) que actúan no determinísticamente.
   La información provista por el productor debe ser retirada siempre 2 veces, es decir que las
   secuencias normales son: ABB, ABC, ACB o ACC. Nota: ¡Ojo con la exclusión mutua!

## Solucion 3

permisoConsumidor = sem(0)
permisoProductor = sem(1)

int producto = 0

A(){

    while(True){
        permisoProductor.wait()
        Producir()
        producto = 2
        permisoConsumidor.signal()
    }

}

Consumen(){

    permisoConsumidor.wait()
    Consumir()
    producto--
    if (producto == 0){
        permisoProductor.signal()
    } else {
        permisoConsumidor.signal()
    }

}

B(){

    while(True){
        Consumen()
    }

}

C(){

    while(True){
       Consumen()
    }

}

4. Se tienen un productor (A) y dos consumidores (B y C). Cuando C retira la información, la
   retira dos veces. Los receptores actúan en forma alternada. Secuencia normal: ABB, AC, ABB,
   AC, ABB, AC...

## Solucion 4

permisoC = sem(0)
permisoB = sem(0)
permisoA = sem(1)

int producto = 0
bool leTocaB = true

A(){

    while(True){
        permisoA.wait()
        Producir()
        producto = 2
        if (leTocaB) {
            permisoB.signal()
        } else {
            permisoC.signal()
        }
    }

}

B(){

    while(True){
        permisoB.wait()
        Consumir()
        producto--
        if (producto == 0){
            leTocaB = false
            permisoA.signal()
        } else {
            permisoB.signal()
        }
    }

}

C(){

    while(True){
        permisoC.wait()
        Consumir()
        producto = 0
        leTocaB = true;
        permisoA.signal()
    }

}
