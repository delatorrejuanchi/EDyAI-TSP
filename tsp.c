#include "tsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DatosTSP* datostsp_crear() {
  DatosTSP* datos = malloc(sizeof(DatosTSP));
  datos->N = 0;
  datos->A = NULL;
  datos->ciudades = slist_crear();

  return datos;
}

int datostsp_cargar(char const* nombreDeArchivo, DatosTSP* datos) {
  FILE* entrada = fopen(nombreDeArchivo, "r");
  if (entrada == NULL) return 1;

  char buffer1[MAX_LONGITUD_CIUDAD], buffer2[MAX_LONGITUD_CIUDAD];

  if (fscanf(entrada, "%s", buffer1) == EOF) {  // "Ciudades"
    fclose(entrada);
    return 2;
  }

  int leyendoCiudades = 1;
  while (leyendoCiudades && fscanf(entrada, "%s", buffer1) != EOF) {
    int longitud = strlen(buffer1);
    if (buffer1[longitud - 1] == ',')
      buffer1[--longitud] = '\0';
    else
      leyendoCiudades = 0;

    char* ciudad = malloc(sizeof(char) * (longitud + 1));
    strcpy(ciudad, buffer1);

    datos->ciudades = slist_agregar_inicio(datos->ciudades, ciudad);
    datos->N++;
  }

  if (fscanf(entrada, "%s\n", buffer1) == EOF) {  // "Costos"
    fclose(entrada);
    return 2;
  }

  datos->A = calloc(datos->N * datos->N, sizeof(int));
  int costo, error = 0;
  while (fscanf(entrada, "%[^,],%[^,],%d\n", buffer1, buffer2, &costo) != EOF &&
         !error) {
    int i = -1, j = -1, indice = 0;
    for (SNodo* iter = datos->ciudades; iter != NULL && (i == -1 || j == -1);
         iter = iter->sig) {
      if (strcmp(buffer1, iter->dato) == 0) i = indice;
      if (strcmp(buffer2, iter->dato) == 0) j = indice;

      indice++;
    }

    if (i == -1 || j == -1)
      error = 1;
    else {
      datos->A[datos->N * i + j] = costo;
      datos->A[datos->N * j + i] = costo;
    }
  }

  fclose(entrada);

  if (error)
    return 3;
  else
    return 0;
}

void datostsp_destruir(DatosTSP* datos) {
  slist_destruir(datos->ciudades);
  free(datos->A);
  free(datos);
}

int* resolver_tsp(DatosTSP* datos) {
  int* A = datos->A;
  int N = datos->N;

  int* solucion = malloc(sizeof(int) * (N + 1));
  solucion[N] = __INT_MAX__;

  int inicio = -1, grado = __INT_MAX__;
  int *costosI = A, gradoI = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++)
      if (costosI[j]) gradoI++;

    if (gradoI < grado) {
      grado = gradoI;
      inicio = i;
    }

    gradoI = 0;
    costosI += N;
  }

  int* costosInicio = A + N * inicio;

  int camino[N], vecinos[grado], iter = 0;
  for (int i = 0; i < N; i++) {
    camino[i] = i;
    if (costosInicio[i]) vecinos[iter++] = i;
  }

  camino[inicio] = 0;
  camino[0] = inicio;

  for (int i = 0; i < grado - 1; i++) {
    int k1 = 1;
    while (camino[k1] != vecinos[i]) k1++;
    int temp1 = camino[1];
    camino[1] = camino[k1];
    camino[k1] = temp1;

    for (int j = i + 1; j < grado; j++) {
      int k2 = 2;
      while (camino[k2] != vecinos[j]) k2++;
      int temp2 = camino[N - 1];
      camino[N - 1] = camino[k2];
      camino[k2] = temp2;

      int costo = costosInicio[vecinos[i]] + costosInicio[vecinos[j]];

      recursion(A, N, solucion, camino, costo, 2);
    }
  }

  return solucion;
}

void recursion(int* A, int N, int* solucion, int* camino, int costo, int i) {
  if (i == (N + 1) / 2) {
    int costo1 = A[N * camino[i - 1] + camino[i]];
    int costoTotal = costo + costo1;
    int valido = costo1;

    if (i == N / 2) {
      int costo2 = A[N * camino[i] + camino[N - i + 1]];
      costoTotal += costo2;
      valido = costo1 && costo2;
    }

    if (valido && costoTotal < solucion[N]) {
      memcpy(solucion, camino, sizeof(int) * N);
      solucion[N] = costoTotal;
    }
  } else {
    int* costosInicio = A + N * camino[i - 1];
    int* costosFinal = A + N * camino[N - i + 1];

    int cotaSuperior = solucion[N] - costo;
    int temp1 = camino[i];

    for (int j = i; j < N - i + 1; j++) {
      int costo1 = costosInicio[camino[j]];

      if (costo1 && costo1 < cotaSuperior) {
        camino[i] = camino[j];
        camino[j] = temp1;

        int temp2 = camino[N - i];

        for (int k = i + 1; k < N - i; k++) {
          int costo2 = costosFinal[camino[k]];

          if (costo2 && costo1 + costo2 < cotaSuperior) {
            camino[N - i] = camino[k];
            camino[k] = temp2;

            recursion(A, N, solucion, camino, costo + costo1 + costo2, i + 1);

            camino[k] = camino[N - i];
          }
        }

        camino[N - i] = temp2;
        int costo2 = costosFinal[temp2];
        if (costo2 && costo1 + costo2 < cotaSuperior) {
          recursion(A, N, solucion, camino, costo + costo1 + costo2, i + 1);
        }

        camino[j] = camino[i];
      }
    }

    camino[i] = temp1;
  }
}

void escribir_solucion(char const* nombreDeArchivo, DatosTSP* datos,
                       int* solucion) {
  FILE* salida = fopen(nombreDeArchivo, "w");

  char *inicio, *ciudadI, *ciudadJ;

  SNodo* iter = datos->ciudades;
  for (int i = 0; i < solucion[0]; i++) iter = iter->sig;
  ciudadI = inicio = iter->dato;

  for (int i = 0; i < datos->N - 1; i++) {
    iter = datos->ciudades;
    for (int j = 0; j < solucion[i + 1]; j++) iter = iter->sig;

    ciudadJ = iter->dato;

    int costo = datos->A[datos->N * solucion[i] + solucion[i + 1]];
    fprintf(salida, "%s,%s,%d\n", ciudadI, ciudadJ, costo);

    ciudadI = ciudadJ;
  }

  int costo = datos->A[datos->N * solucion[datos->N - 1] + solucion[0]];
  fprintf(salida, "%s,%s,%d\n", ciudadI, inicio, costo);

  fclose(salida);
}
