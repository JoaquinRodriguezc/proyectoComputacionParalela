### `int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,MPI_Comm comm)`

#### Parámetros de entrada

- **sendbuf**: Dirección del búfer de envío (solo relevante en el proceso raíz).
- **sendcount**: Número de elementos enviados a cada proceso (entero, solo relevante en el proceso raíz).
- **sendtype**: Tipo de dato de los elementos del búfer de envío (solo relevante en el proceso raíz) (handle).
- **recvbuf**: Dirección del búfer de recepción.
- **recvcount**: Número de elementos en el búfer de recepción (entero).
- **recvtype**: Tipo de dato de los elementos del búfer de recepción (handle).
- **root**: Rango del proceso que envía los datos (entero).
- **comm**: Comunicador (handle).

### `int MPI_Scatterv(sendbuf, sendcounts, displs, sendtype,recvbuf, recvcount, recvtype,root, comm)`

En comparación con `MPI_Scatter`, existen dos diferencias principales:

1. El argumento **sendcounts** ahora es un array.
2. Se ha agregado un nuevo argumento **displs**.

#### Parámetros

- **sendbuf**: Dirección inicial del búfer de envío.
- **sendcounts**: Array de enteros que especifica la cantidad de elementos a enviar desde el proceso `root` a cada proceso. Su valor es significativo solo en `root`.
- **displs**: Array que especifica el desplazamiento relativo desde `sendbuf` para iniciar los datos que se enviarán a cada proceso correspondiente, en unidades de `sendtype`. Solo es significativo en `root`.
- **sendtype**: Tipo de dato de los elementos en el búfer de envío.
- **recvbuf**: Dirección del búfer de recepción.
- **recvcount**: Número de elementos en el búfer de recepción.
- **recvtype**: Tipo de dato de los elementos en el búfer de recepción.
- **root**: Rango del proceso que envía los datos.
- **comm**: Comunicador del grupo.

### `int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[],MPI_Datatype recvtype, int root, MPI_Comm comm)`

### Parámetros de Entrada

- **sendbuf**: Dirección inicial del búfer de envío (de elección).
- **sendcount**: Número de elementos en el búfer de envío (entero no negativo).
- **sendtype**: Tipo de dato de los elementos en el búfer de envío (manejador).
- **recvcounts**: Array de enteros no negativos (de tamaño igual al número de procesos en el grupo) que contiene la cantidad de elementos recibidos de cada proceso.
- **displs**: Array de enteros (de tamaño igual al número de procesos en el grupo). Cada entrada `i` especifica el desplazamiento relativo a `recvbuf` donde se colocarán los datos recibidos del proceso `i`.
- **recvtype**: Tipo de dato de los elementos en el búfer de recepción (manejador).
- **root**: Rango del proceso que recibe los datos (entero).
- **comm**: Comunicador del grupo (manejador).
