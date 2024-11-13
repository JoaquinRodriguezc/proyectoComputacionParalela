#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_NUMBERS 100000000

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = low - 1;
    int j;
    for (j = low; j < high; j++)
    {
        if (array[j] <= pivot)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return i + 1;
}

void quickSort(int array[], int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(array, low, high);
        quickSort(array, low, pivotIndex - 1);
        quickSort(array, pivotIndex + 1, high);
    }
}
void mergeSortedFragments(int *array, int totalSize, int *fragmentSizes, int worldSize)
{
    int *tempArray = (int *)malloc(totalSize * sizeof(int));
    int currentIndices[worldSize];
    int fragmentStartIndices[worldSize];
    int i, j;

    printf("Proceso 0: Iniciando fusión de fragmentos\n");

    /*
        Cada fragmento va a tener un índice que apunte a un elemento dentro su fragmento.
        Se inicializan en el comienzo de cada fragmento
    */
    fragmentStartIndices[0] = 0;
    for (i = 0; i < worldSize; i++)
    {
        currentIndices[i] = fragmentStartIndices[i];
        if (i < worldSize - 1)
        {
            fragmentStartIndices[i + 1] = fragmentStartIndices[i] + fragmentSizes[i];
        }
    }

    for (i = 0; i < totalSize; i++)
    {
        int minValue = 1000000;
        int minIndex = -1;

        /**
         * Se ve qué valores tienen los fragmentos en los lugares donde apuntan los indices
         * y se ve quien tiene el valor más bajo de esos.
         * Al fragmento que tenga el valor más bajo se le suma uno a su índice y el valor minimo que tenia
         * se copia al array temporal
         */
        for (j = 0; j < worldSize; j++)
        {
            if (currentIndices[j] < fragmentStartIndices[j] + fragmentSizes[j] && array[currentIndices[j]] < minValue)
            {
                minValue = array[currentIndices[j]];
                minIndex = j;
            }
        }
        tempArray[i] = minValue;
        currentIndices[minIndex]++;
    }

    for (i = 0; i < totalSize; i++)
    {
        array[i] = tempArray[i];
    }
    free(tempArray);
    printf("Proceso 0: Fusión de fragmentos completada\n");
}

int main(int argc, char **argv)
{
    int processRank, numeroProcesos;
    double startTime, endTime;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProcesos);

    printf("Proceso %d: Iniciando ejecución\n", processRank);

    int numeroElementos = 0;
    int *array = NULL;
    int *localFragment = NULL;
    int *fragmentSizes = NULL;
    int *offsets = NULL;
    int i;

    if (processRank == 0)
    {

        // Generamos un array con un rango de 0 a 1.000.000
        array = (int *)malloc(MAX_NUMBERS * sizeof(int));
        printf("Proceso 0: Generando datos aleatorios\n");
        for (numeroElementos = 0; numeroElementos < MAX_NUMBERS; numeroElementos++)
        {
            array[numeroElementos] = rand() % 1000000;
        }

        printf("Proceso 0: Generación de datos completada, total de elementos: %d\n", numeroElementos);

        // Iniciamos cronómetro
        startTime = MPI_Wtime();

        /**
         * El algoritmo paralelo que elegimos primero descompone la información en fragmentos
         * y luego le envia un fragmento a cada proceso disponible.
         * Entonces para eso se necesita calcular:
         * fragmentSizes: cuantos elementos tiene cada fragmento
         * offset: indica dónde empieza el fragmento
         */

        fragmentSizes = (int *)malloc(numeroProcesos * sizeof(int));
        offsets = (int *)malloc(numeroProcesos * sizeof(int));

        int baseFragmentSize = numeroElementos / numeroProcesos;
        int remainingElements = numeroElementos % numeroProcesos;

        for (i = 0; i < numeroProcesos; i++)
        {
            fragmentSizes[i] = baseFragmentSize + (i < remainingElements ? 1 : 0);
            offsets[i] = (i == 0) ? 0 : offsets[i - 1] + fragmentSizes[i - 1];
            printf("Proceso 0: Fragmento %d tiene tamaño %d, desplazamiento %d\n", i, fragmentSizes[i], offsets[i]);
        }
    }

    /* Se le envia a cada proceso el tamaño del fragmento
    que va a recibir y este lo guarda
    */
    int localFragmentSize;

    MPI_Scatter(fragmentSizes, 1, MPI_INT, &localFragmentSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /*
     Cada proceso guarda el espacio para recibir ese fragmento
    */

    localFragment = (int *)malloc(localFragmentSize * sizeof(int));
    /*
    Una vez que los procesos guardaron la memoria suficiente,
    se les envia el fragmento correspondiente
    */
    MPI_Scatterv(array, fragmentSizes, offsets, MPI_INT, localFragment, localFragmentSize, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Proceso %d: Recibido fragmento de tamaño %d\n", processRank, localFragmentSize);

    /*
    Cada proceso usa quicksort secuencial para ordenar su fragmento
    */
    quickSort(localFragment, 0, localFragmentSize - 1);
    printf("Proceso %d: Fragmento ordenado\n", processRank);

    /*
    Se juntan todos los fragmentos
    */
    MPI_Gatherv(localFragment, localFragmentSize, MPI_INT, array, fragmentSizes, offsets, MPI_INT, 0, MPI_COMM_WORLD);

    if (processRank == 0)
    {
        /*
         * Hasta acá tenemos N cantidad de fragmentos ordenados pero ahora hay que ordenar
         hacer que esos fragmentos estén en orden comparados unos con otros.
         Es decir, hay que mergearlos en orden.
         */

        mergeSortedFragments(array, numeroElementos, fragmentSizes, numeroProcesos);

        /*Paramos de contar el tiempo de ejecución*/
        endTime = MPI_Wtime();

        // Calculamos el tiempo en segundos, milisegundos y nanosegundos
        double time_spent_sec = endTime - startTime;
        double time_spent_ms = time_spent_sec * 1e3;
        double time_spent_ns = time_spent_sec * 1e9;

        printf("Tiempo de ejecución desde la división hasta la fusión:\n");
        printf("  %.9f segundos\n", time_spent_sec);
        printf("  %.6f milisegundos\n", time_spent_ms);
        printf("  %.0f nanosegundos\n", time_spent_ns);

        // Liberamos la memoria en el root
        free(array);
        free(fragmentSizes);
        free(offsets);
    }

    // Liberamos la mem de cada proceso
    free(localFragment);
    printf("Proceso %d: Finalizando ejecución\n", processRank);
    MPI_Finalize();
    return 0;
}