#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define false 0
#define true 1
#define MAX_N 100000
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int array[], int low, int high)
{
    int pivot = array[high]; // Elige el último elemento como pivote
    int leftIndex = low - 1; // Índice de la izquierda para encontrar elementos mayores que el pivote
    int rightIndex = high;   // Índice de la derecha para encontrar elementos menores que el pivote

    while (1)
    {
        do
        {
            leftIndex++;
        } while (array[leftIndex] < pivot);
        do
        {
            rightIndex--;
        } while (rightIndex >= low && array[rightIndex] > pivot);

        if (leftIndex >= rightIndex)
        {
            swap(&array[leftIndex], &array[high]);
            return leftIndex;
        }
        swap(&array[leftIndex], &array[rightIndex]);
    }
}
int isPrime(int n)
{
    if (n == 1 || n == 0)
        return false;

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}
int countPrimes(int array[], int size)
{
    int primeCounter = 0;
    for (int i = 0; i < size; i++)
    {
        if (isPrime(array[i]))
        {

            primeCounter++;
        }
    }
    return primeCounter;
}
void quickSort(int array[], int bajo, int alto)
{
    if (bajo < alto)
    {
        int pivot = partition(array, bajo, alto);
        quickSort(array, bajo, pivot - 1);
        quickSort(array, pivot + 1, alto);
    }
}

void printArray(int array[], int largoArray)
{
    int i;
    for (i = 0; i < largoArray; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Tenés que ingresar la cantidad de números random a ordenar\n");
        exit(1);
    }
    long MAX_NUMEROS = strtol(argv[1], NULL, 10);
    int num, i = 0;
    int *array = (int *)malloc(MAX_NUMEROS * sizeof(int));
    for (i; i < MAX_NUMEROS; i++)
    {
        array[i] = rand() % MAX_N;
    }
    struct timespec startSorting, endSorting;
    clock_gettime(CLOCK_MONOTONIC, &startSorting);

    quickSort(array, 0, MAX_NUMEROS - 1);

    clock_gettime(CLOCK_MONOTONIC, &endSorting);

    double exec_time_sorting = (endSorting.tv_sec - startSorting.tv_sec) + (endSorting.tv_nsec - startSorting.tv_nsec) / 1e9;

    struct timespec startFindPrimes, endFindPrimes;
    clock_gettime(CLOCK_MONOTONIC, &startFindPrimes);
    int quantity = countPrimes(array, MAX_NUMEROS);
    clock_gettime(CLOCK_MONOTONIC, &endFindPrimes);
    free(array);
    double exec_time_finding = (endFindPrimes.tv_sec - startFindPrimes.tv_sec) + (endFindPrimes.tv_nsec - startFindPrimes.tv_nsec) / 1e9;
    printf("\n Cantidad de numeros a ordenar: %ld", MAX_NUMEROS);
    printf("\n Cantidad de primos: %i", quantity);
    printf("\n Tiempo de ejecución para buscar los primos: %.9fs", exec_time_finding);
    printf("\n Tiempo de ejecución para ordenar el array: %.9fs", exec_time_sorting);
    printf("\n Tiempo total: %.9fs\n", exec_time_finding + exec_time_sorting);
    return 0;
}
