#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct data
{
    int id;    
    char** palabras;
    int cantidad_palabras;
    int numero_filas;
    int numero_columnas;
} data;

char ** palabras(char* archivo, int num_lineas);
char ** crearMatriz(int i, int j);
char ** inicializarMatriz(int i, int j, char** matriz);
int contar(char *nombre);
int* asignar(int num, int num_hilos);
void liberarMatriz(int i, int j, char ** matriz);
void imprimirMatriz(int i, int j, char ** matriz);
void imprimirPalabras(char** palabras, int num_lineas);
char ** rellenarBasura(char** matriz, int n, int m);
void escribirSalida(char* salida, char** matriz, int num_lineas, int num_columnas);
data* asignarData(char** palabras, int* asignacion, int num_lineas, int num_columnas, int num_filas, int num_hebras);
void imprimirData(data* datos, int num_hebras);
pthread_mutex_t* crearMutex(int num_lineas);
int randomMax(int numero);
int largoPalabra(char* palabra);
void *ubicar(void *params);

