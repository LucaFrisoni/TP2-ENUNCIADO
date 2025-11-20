#include "cola.h"
#include "lista.h"

struct cola {
	lista_t *lista;
};

cola_t *cola_crear()
{
	lista_t *lista = lista_crear();
	if (!lista)
		return NULL;

	cola_t *cola = calloc(1, sizeof(cola_t));
	if (!cola) {
		free(lista);
		return NULL;
	}

	cola->lista = lista;
	return cola;
}

bool cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola)
		return false;
	return lista_agregar(cola->lista, elemento);
}

void *cola_desencolar(cola_t *cola)
{
	if (!cola)
		return NULL;
	return lista_eliminar_elemento(cola->lista, 0);
}

void *cola_ver_primero(cola_t *cola)
{
	if (!cola || lista_cantidad(cola->lista) == 0)
		return NULL;

	return lista_buscar_elemento(cola->lista, 0);
}

size_t cola_cantidad(cola_t *cola)
{
	if (!cola)
		return 0;
	return lista_cantidad(cola->lista);
}

void cola_destruir(cola_t *cola)
{
	if (!cola)
		return;
	lista_destruir(cola->lista);
	free(cola);
}