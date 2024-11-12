#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define false 0
#define true 1
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int array[], int bajo, int alto)
{
    int pivot = array[alto];
    int i = bajo - 1;
    int j;
    for (j = bajo; j <= alto - 1; j++)
    {
        if (array[j] <= pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[alto]);
    return i + 1;
}
int countPrimes(int array[], int length)
{
    int i = 0;
    int primesQuantity = 0;
    for (i; i < length; i++)
    {
        int j = array[i] - 1;
        int isPrime = true;
        if (array[i] <= 1)
        {

            isPrime = false;
        }

        while (isPrime == true && j > 1)
        {
            if (array[i] % j == 0)
            {
                isPrime = false;
            }
            j--;
        }
        if (isPrime)
        {
            primesQuantity++;
        }
    }
    return primesQuantity;
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
        printf("Tenés que ingresar la cantidad de números random a ordenar");
        exit(1);
    }
    long MAX_NUMEROS = strtol(argv[1], NULL, 10);
    int num, i = 0;
    int *array = (int *)malloc(MAX_NUMEROS * sizeof(int));
    for (i; i < MAX_NUMEROS; i++)
    {
        array[i] = rand() % 1000000;
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
    double exec_time_finding = (endFindPrimes.tv_sec - startFindPrimes.tv_sec) + (endFindPrimes.tv_nsec - startFindPrimes.tv_nsec) / 1e9;
    printf("\n Cantidad de numeros a ordenar: %ld", MAX_NUMEROS);
    printf("\n Cantidad de primos: %i", quantity);
    printf("\n Tiempo de ejecución para buscar los primos: %.9fs", exec_time_finding);
    printf("\n Tiempo de ejecución para ordenar el array: %.9fs", exec_time_sorting);
    printf("\n Tiempo total: %.9fs\n", exec_time_finding + exec_time_sorting);
    return 0;
}
