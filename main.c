#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tsp.h"

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Error. El número de argumentos ingresado es incorrecto.\n");
    printf("Modo de uso: %s archivoEntrada archivoSalida\n", argv[0]);
    return 1;
  }

  DatosTSP* datos = datostsp_crear();
  int error = datostsp_cargar(argv[1], datos);
  if (error) {
    switch (error) {
      case 1:
        printf("Error. El archivo %s no existe.\n", argv[1]);
        break;
      case 2:
        printf("Error. El formato del archivo %s es incorrecto.\n", argv[1]);
        break;
      case 3:
        printf("Error. Uno de los lados contiene una ciudad inexistente.\n");
    }

    datostsp_destruir(datos);
    return 1;
  } else {
    printf("El TSP de %s ha sido leído correctamente.\n", argv[1]);
  }

  clock_t timer = clock();

  int* solucion = resolver_tsp(datos);

  timer = clock() - timer;

  error = solucion[datos->N] == __INT_MAX__;
  if (error)
    printf("Error. El TSP de %s no tiene solución.\n", argv[1]);
  else {
    printf("Se ha encontrado una solución con costo %d en %lfs.\n",
           solucion[datos->N], ((double)timer) / CLOCKS_PER_SEC);
    escribir_solucion(argv[2], datos, solucion);
    printf("La solución al TSP ha sido escrita en %s.\n", argv[2]);
  }

  free(solucion);
  datostsp_destruir(datos);

  if (error)
    return 1;
  else
    return 0;
}
