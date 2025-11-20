#include "pila.h"
#include "lista.h"

struct pila {
	lista_t *lista;
};

pila_t *pila_crear()
{
	lista_t *lista = lista_crear();
	if (!lista)
		return NULL;

	pila_t *pila = calloc(1, sizeof(pila_t));
	if (!pila) {
		free(lista);
		return NULL;
	}

	pila->lista = lista;
	return pila;
}

bool pila_apilar(pila_t *pila, void *elemento)
{
	if (!pila)
		return false;

	if (lista_vacia(pila->lista))
		return lista_agregar(pila->lista, elemento);

	return lista_insertar(pila->lista, elemento, 0);
}

void *pila_desapilar(pila_t *pila)
{
	if (!pila)
		return NULL;
	return lista_eliminar_elemento(pila->lista, 0);
}

void *pila_ver_primero(pila_t *pila)
{
	if (!pila || lista_cantidad(pila->lista) == 0)
		return NULL;

	return lista_buscar_elemento(pila->lista, 0);
}

size_t pila_cantidad(pila_t *pila)
{
	if (!pila)
		return 0;
	return lista_cantidad(pila->lista);
}

void pila_destruir(pila_t *pila)
{
	if (!pila)
		return;
	lista_destruir(pila->lista);
	free(pila);
}