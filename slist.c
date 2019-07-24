#include "slist.h"
#include <stdlib.h>

SList slist_crear() { return NULL; }

int slist_vacia(SList slist) { return slist == NULL; }

SList slist_agregar_inicio(SList slist, char* dato) {
  SNodo* nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = slist;

  return nuevoNodo;
}

SList slist_eliminar_inicio(SList slist) {
  SNodo* nodoAEliminar = slist;
  slist = slist->sig;

  free(nodoAEliminar->dato);
  free(nodoAEliminar);

  return slist;
}

void slist_destruir(SList slist) {
  while (!slist_vacia(slist)) {
    slist = slist_eliminar_inicio(slist);
  }
}
