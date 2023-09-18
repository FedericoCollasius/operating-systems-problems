Algoritmos de Scheduling que pueden resultar en starvation:

- SJF: entran procesos que tienen el tiempo mas corto entre todos los procesos. Los procesos con tiempo mayor no corren.
- SRTF: caso similar. Los procesos que entran tienen un remaining time menor a todos los demas procesos.
- RR por prioridad: Si constantemente entra un proceso con mayor prioridad los demas no corren.
- Colas de multinivel: entran procesos prioritarios, los demas no corren.

Algoritmos de Scheduling que no pueden resultar en starvation:

- RR: quantum fijo y cola circular.
- FIFO: es igual que RR solo que sin un quantum fijo, los procesos se ejecutan en orden hasta que terminen.
- Colas de multinivel con aging: los procesos que todavia no corrieron ganan prioridad y eventualmente quedan en la primera cola.
