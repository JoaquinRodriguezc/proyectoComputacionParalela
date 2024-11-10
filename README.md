# QuickSort en C. Versión Secuencial y Paralela (usando MPI)
## Qué es quicksort
Es un algoritmo de ordanamienento. Tiene un tiempo de ejecución de O(n^2) en el peor de los casos, lo que ocurre cuando el arreglo está ordenado en orden descendente. Aunque esto suena malo, en la práctica es uno de los algoritmos más rápidos para ordenar. En promedio, su tiempo de ejecución es O(n log n), y además, tiene la ventaja de ordenar directamente en el mismo arreglo, sin necesitar espacio extra.

## Intrucciones para correr la versión secuencial
`gcc -o quicksort quicksort.c ` 
 ` ./quicksort ` 
## Instrucciones para correr la versión paralela en cluster

### Levantar máquina virtual 
`mpdboot -n (cantidad nodos activos) -f nodoscluster`
### Compilar y ejectutar
`mpicc -o quicksortParalelo quicksortParalelo.c`

`mpiexec -n (cantidad procesos) ./quicksortParalelo`


