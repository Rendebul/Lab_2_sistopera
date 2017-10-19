#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "funciones.h"

char **matriz;

int main(int argc, char **argv)
{
  //Se declaran variables
  char *nombre_entrada;
  char *nombre_salida;
  int numero_filas, numero_columnas, numero_hebras = 0;
  int c;
  bool mostrar = false;


  opterr = 0;

    //Se capta el getopt correspondiente al coordinador.
  while ((c = getopt (argc, argv, "i:h:n:m:s:d::")) != -1)
    switch (c) {
        case 'n':
                    numero_filas = atoi(optarg);
                    break;
        case 'm':
                    numero_columnas = atoi(optarg);
                    break;
        case 'h':
                    numero_hebras = atoi(optarg);
                    break;
        case 'i':
                    nombre_entrada = optarg;
                    break;
        case 's':
                    nombre_salida = optarg;
                    break;
        case 'd':
                    mostrar = true;
                    break;
        case '?':
                    if (optopt == 'i' || optopt == 'n' || optopt == 'c' || optopt == 'h' || optopt == 's')
                        fprintf (stderr, "La opcion -%c requires un argumento.\n", optopt);
                    else if (isprint (optopt))
                        fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
                    else
                        fprintf (stderr,"Caracter desconocido `\\x%x'.\n",optopt);
                    return 1;
        default:
                abort ();
  }
  printf ("i = %s, s= %s, n = %d, m = %d, h = %d, d = %d\n",nombre_entrada, nombre_salida, numero_filas, numero_columnas, numero_hebras, mostrar);
  if(numero_filas < 1){
    printf("Tamaño incorrecto para inicializar tablero\n");
    abort();
  }
  if(numero_columnas < 1){
    printf("Tamaño incorrecto para inicializar tablero\n");
    abort();
  }
  matriz = crearMatriz(numero_filas,numero_columnas);
  matriz = inicializarMatriz(numero_filas, numero_columnas, matriz);
  matriz = rellenarBasura(matriz, numero_filas, numero_columnas);
  imprimirMatriz(numero_filas, numero_columnas, matriz);
  int cantidad_lineas = contar(nombre_entrada);
  char **total_palabras = palabras(nombre_entrada, cantidad_lineas);
  
  //imprimirPalabras(total_palabras, cantidad_lineas);
  //Cuento la cantidad de lineas para luego asignar carga equitativa por proceso.
  //int num_lineas = contar(nombre_entrada, numero_caracteres);

  //Se asigna la cantidad de lineas acorde a los procesos.
  int *asignacion = asignar(cantidad_lineas, numero_hebras);
  data *arregloDatos = asignarData(total_palabras, asignacion, cantidad_lineas, numero_hebras);
  imprimirData(arregloDatos, numero_hebras);
  //escribirSalida(nombre_salida, matriz, numero_filas, numero_columnas);

  liberarMatriz(numero_filas, numero_columnas, matriz);
  return 0;
}