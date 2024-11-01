#include <stdio.h>
#include <time.h>

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
    for (int j = bajo; j <= alto - 1; j++)
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
    for (int i = 0; i < largoArray; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    int arr[] = {10, 4, 7, 2, 8, 9, 1, 5, 6, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array inicial: ");
    printArray(arr, n);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    quickSort(arr, 0, n - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double exec_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nArray ordenado: ");
    printArray(arr, n);

    printf("\nTiempo de ejecucion: %.9f segundos\n", exec_time);

    return 0;
}
