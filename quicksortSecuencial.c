#include <stdio.h>
#include <time.h>
#define MAX_NUMEROS 1000
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

int main()
{
    FILE *punteroArchivo = fopen("./numerosRandom.txt", "r");
    int num, i = 0;
    int array[MAX_NUMEROS];
    if (punteroArchivo == NULL)
    {
        printf("Por favor, incluir archivo 'numerosRandom.txt en el directorio'");
        return 0;
    }
    while (fscanf(punteroArchivo, "%d", &num) != EOF && i < MAX_NUMEROS)
    {
        array[i++] = num;
    }

    printf("Array sin ordenar: ");
    printArray(array, MAX_NUMEROS);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    quickSort(array, 0, MAX_NUMEROS - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double exec_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nArray ordenado: ");
    printArray(array, MAX_NUMEROS);

    printf("\nTiempo de ejecucion: %.9f milisegundos\n", exec_time * 1000);

    return 0;
}
