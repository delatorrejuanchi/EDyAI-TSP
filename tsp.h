#if !defined(__TSP__)
#define __TSP__

#include "slist.h"

// Definimos una longitud máxima para el nombre de las ciudades
#define MAX_LONGITUD_CIUDAD 50

// Una estructura DatosTSP almacena los datos de un Traveling Salesman Problem
// int N: cantidad de ciudades
// SList ciudades: lista con los nombres de las ciudades (*)
// int* A: matriz de costos asociada al problema (**)

// (*) Asociamos a cada ciudad un índice, de acuerdo a la posición en la que se
// encuentra en esta lista.

// (**) La matriz de costos asociada al problema es una matriz, A, cuya entrada
// en la fila i y la columna j es A[i][j] y representa:
//   - Si A[i][j] = 0: la ciudad i-ésima y la ciudad j-ésima no se encuentran
//                     unidas directamente.
//   - Si A[i][j] != 0: A[i][j] representa el costo de ir de la ciudad i-ésima a
//                      la ciudad j-ésima.

// Dado que estamos resolviendo un Traveling Salesman Problem simétrico, en el
// que el costo de ir de la ciudad i-ésima a la j-ésima es equivalente al de ir
// de la ciudad j-ésima a la i-ésima, resulta evidente que la matriz de costos
// asociada al problema es una matriz simétrica.

// Representamos esta matriz de costos mediante un arreglo de enteros tamaño
// N*N, y por eso accedemos a la entrada en la fila i y columna j mediante
// A[N * i + j].
typedef struct {
  int N, *A;
  SList ciudades;
} DatosTSP;

// Representamos la solución a un Traveling Salesman Problem mediante un arreglo
// de enteros de N + 1 elementos. Los primeros N elementos representan el orden
// en que se deben recorrer las ciudades para visitarlas todas con el menor
// costo posible. El último elemento representa el costo total de dicho
// recorrido.

// datostsp_crear: -> DatosTSP*
// Crea y devuelve un puntero a una estructura DatosTSP.
DatosTSP* datostsp_crear();

// datostsp_cargar: (char const*) DatosTSP* -> int
// Recibe el nombre de un archivo de entrada y un puntero a una estructura
// DatosTSP,
// Si el archivo de entrada no existe, devuelve 1.
// En caso contrario, carga los datos del Traveling Salesman Problem de acuerdo
// al archivo de entrada.
// Si el archivo finaliza inesperadamente, devuelve 2.
// Si el archivo contiene lados con ciudades inexistentes, devuelve 3.
// Si el formato del archivo es correcto, devuelve 0.
// Se asume que el archivo de entrada tiene el siguiente formato:
//   - La primera línea contiene el encabezado "Ciudades"
//   - En la segunda línea, se encuentran los nombres de las ciudades separados
//     por una coma y un espacio (*). Por ejemplo:
//       "ciudad_1, ciudad_2, ciudad_3"
//   - La tercer línea contiene el encabezado "Costos"
//   - El resto de las líneas tienen el costo de ir de la ciudad i-ésima a la
//     ciudad j-ésima de la siguiente manera:
//       "ciudad_i,ciudad_j,X"
//     En donde ciudad_i y ciudad_j son los nombres de ambas ciudades y X es un
//     entero que representa el costo de ir de la ciudad i-ésima a la j-ésima.
// (*) Los nombres de las ciudades deben tener menos de 50 caracteres.
int datostsp_cargar(char const* nombreDeArchivo, DatosTSP* datos);

// datostsp_destruir: DatosTSP* datos -> void
// Recibe un puntero a una estructura DatosTSP,
// la destruye.
void datostsp_destruir(DatosTSP* datos);

// resolver_tsp: DatosTSP* int* -> void
// Recibe un puntero a una estructura DatosTSP,
// Resuelve el Traveling Salesman Problem asociado y devuelve su solución.
int* resolver_tsp(DatosTSP* datos);

void recursion(int* A, int N, int* solucion, int* camino, int costo, int i);
void recursion_izq(int* A, int N, int* solucion, int* camino, int costo, int i,
                   int f);
void recursion_der(int* A, int N, int* solucion, int* camino, int costo, int i,
                   int f);

// escribir_solucion: (char const*) DatosTSP* int* -> void
// Recibe el nombre de un archivo de salida, un puntero a una estructura
// DatosTSP, y la solución al Traveling Salesman Problem asociado.
// Escribe un archivo de salida con los lados que forman la solución, y sus
// respectivos costos. Cada línea es de la forma "ciudad_i,ciudad_j,X", en donde
// ciudad_i y ciudad_j son los nombres de ambas ciudades y X es un entero que
// representa el costo de ir de la ciudad i-ésima a la j-ésima.
void escribir_solucion(char const* nombreDeArchivo, DatosTSP* datos,
                       int* solucion);

#endif  // __TSP__
