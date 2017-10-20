#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "funciones.h"
#include <pthread.h>

char ** crearMatriz(int i, int j){
    char ** matriz = (char**)malloc(sizeof(char*)*i);
    if(!matriz) {
        printf("Error al asignar memoria\n");
        exit(-1);
    }

    for (int aux = 0; aux < i; ++aux)
    {
        matriz[aux] = (char*)malloc(sizeof(char)*j);
        if(!matriz[aux]) {
            printf("Error al asignar memoria\n");
            exit(-1);
        }
    }
    return matriz;
};

char ** inicializarMatriz(int i, int j, char **matriz) {
    for (int aux = 0; aux < i; ++aux)
    {
        for (int aux2 = 0; aux2 < j; ++aux2)
        {   
            matriz[aux][aux2] = ' ';
        }
    }

    return matriz;
};

//Funcion que cuenta las lineas.
int contar(char *nombre) {
    FILE *fa;
    int contador = 0;
    fa = fopen(nombre, "r");
    if(fa==NULL) {
        printf("Error al abrir de entrada\n");
        abort();
    }
    char *linea = malloc(sizeof(char)*(255));
    while(fgets(linea, 255, fa) != NULL) {
        contador++;
    }
    fclose(fa);
    free(linea);
    return contador;
};

//Funcion que asigna la cantidad de lineas a leer segun cantidad de procesos
//A partir de la diferencia entre funcion piso y cielo.
int* asignar(int num, int num_procesos) {
    int piso = floor((double)num/num_procesos);
    int *asignacion = malloc(sizeof(int)*num_procesos);
    int diferencia = (num%num_procesos);
    for (int i = 0; i < num_procesos; ++i)
    {
        asignacion[i] = piso;
    }

    for (int i = 0; i < diferencia; ++i)
    {
        asignacion[i] += 1;
    }

    return asignacion;
};

void liberarMatriz(int i, int j, char ** matriz) {
    for (int linea = 0; linea < i; ++linea)
    {
        free(matriz[linea]);
    }
};

void imprimirMatriz(int i, int j, char ** matriz) {
    for (int linea = 0; linea < i; ++linea)
    {
        for (int columna = 0; columna < j; ++columna)
        {
           printf("%c", matriz[linea][columna]);
        }
        printf("\n");
    }
};

char** palabras(char* archivo,int num_lineas) {
    FILE *fa;
    int contador = 0;
    fa = fopen(archivo, "r");
    if(fa==NULL) {
        printf("Error al abrir de entrada\n");
        abort();
    }
    char **palabras = (char**) malloc(sizeof(char*)*num_lineas);
    char *linea = malloc(sizeof(char)*(20));
    int i = 0;
    while(fgets(linea, 20, fa) != NULL) {
        palabras[i] = (char *)malloc(sizeof(char)*20);
        memset(palabras[i],' ',sizeof(palabras));
        strtok(linea, "\n");
        strcpy(palabras[i],linea);
        i++;
    }
    fclose(fa);
    free(linea);
    return palabras;
};

void imprimirPalabras(char** palabras, int num_lineas) {
    for (int i = 0; i < num_lineas; ++i)
    {
        printf("%s\n", palabras[i]);
    }
};

char** rellenarBasura(char**matriz, int n, int m) {
    for (int i = 0; i < n; ++i)
    {   
        for (int j = 0; j < m; ++j)
        {
            if(matriz[i][j] == ' ') {
                matriz[i][j] = random()%26+97;
            }
        }
    }
    return matriz;
};

void escribirSalida(char* salida, char** matriz, int num_lineas, int num_columnas) {
    FILE *fa;
    fa = fopen(salida, "w");
    if(fa==NULL) {
        printf("Error al abrir el archivo\n");
        abort();
    }
    printf("Hola\n");
    fflush(fa);
    for (int i = 0; i < num_lineas; ++i)
    {
        for (int j = 0; j < num_columnas; ++j)
        {
            fputc(matriz[i][j],fa);
        }
        fputc('\n',fa);
    }

    fclose(fa);
};

data* asignarData(char** palabras, int* asignacion, int num_lineas,int num_columnas,int num_filas ,int num_hebras) {
    data* arregloDatos = (data*)malloc(sizeof(data)*num_hebras);
    int palabra_ant = 0;
    for (int i = 0; i < num_hebras; ++i)
    {   
        char** palabras_asignadas = (char**)malloc(sizeof(char*)*asignacion[i]);
        int cant_palabras = 0;
        for (int j = 0; j < asignacion[i]; ++j)
        {
            palabras_asignadas[j] = (char*)malloc(sizeof(palabras[palabra_ant]));
            memset(palabras_asignadas[j],' ',sizeof(palabras_asignadas));
            strcpy(palabras_asignadas[j],palabras[palabra_ant]);
            cant_palabras += 1;
            palabra_ant++;
        }
        data dato = {i,palabras_asignadas, cant_palabras, num_filas, num_columnas};
        arregloDatos[i] = dato;
    }
    return arregloDatos;
}

void imprimirData(data* datos, int num_hebras){
    for (int i = 0; i < num_hebras; ++i)
    {
        printf("Impresión datos:\n");
        printf("id: %d\n", datos[i].id);
        printf("cantidad palabras: %d\n", datos[i].cantidad_palabras);
        for (int j  = 0; j   < datos[i].cantidad_palabras; ++j)
        {
            printf("palabras: %s\n", datos[i].palabras[j]);
        }
    }
}

pthread_mutex_t* crearMutex(int num_lineas){
    pthread_mutex_t* matriz = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t*)*num_lineas);

    for (int i = 0; i < num_lineas; ++i)
    {
        pthread_mutex_init(&matriz[i], NULL);
    }
    return matriz;
}

int randomMax(int max) {
    srand(time(NULL));
    return rand()%max;
}

int largoPalabra(char* palabra) {
    int iterator = 0;
    while(palabra[iterator] != 0) {
        iterator++;
    }
    return iterator-1;
}

void *ubicar(void *params) {
    printf("Hola!\n");
    data* datos = (data *)params;
    int num_filas = (int)datos->numero_filas;
    int num_columnas = (int) datos->numero_columnas;
    char ** palabras = (char**)datos->palabras;
    int num_palabras = (int) datos->cantidad_palabras;
}

int validarEscritura(){}

