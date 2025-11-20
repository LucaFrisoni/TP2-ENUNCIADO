#include "lista.h"

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *primero;
	nodo_t *ultimo;
	size_t cantidad;
};

struct lista_iterador {
	nodo_t *actual;
};

// ------------------------------------------------------------------------------------------------------------

static nodo_t *creando_nodo()
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo)
		return NULL;
	return nuevo_nodo;
}

nodo_t *buscando_nodo_por_pos(lista_t *lista, size_t posicion, nodo_t **previo)
{
	if (!lista || posicion >= lista->cantidad)
		return NULL;

	nodo_t *nodo_actual = lista->primero;
	nodo_t *nodo_previo = NULL;

	for (size_t i = 0; i < posicion; i++) {
		nodo_previo = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	if (previo)
		*previo = nodo_previo;

	return nodo_actual;
}

// ------------------------------------------------------------------------------------------------------------Lista
lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));
	if (!lista)
		return NULL;

	return lista;
}
//-----------------------------------------------
bool lista_vacia(lista_t *lista)
{
	if (!lista)
		return true;

	return (lista->primero == NULL && lista->ultimo == NULL);
}
//-----------------------------------------------
size_t lista_cantidad(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}
//-----------------------------------------------
bool lista_agregar(lista_t *lista, void *dato)
{
	if (!lista)
		return false;

	nodo_t *nuevo_nodo = creando_nodo();
	if (!nuevo_nodo)
		return false;

	nuevo_nodo->elemento = dato;
	if (!lista->primero) { // lista vacía
		lista->primero = nuevo_nodo;
	} else { // lista con elementos
		lista->ultimo->siguiente = nuevo_nodo;
	}
	lista->ultimo = nuevo_nodo;

	lista->cantidad++;
	return true;
}
//-----------------------------------------------
bool validando_lista_y_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista))
		return false;
	if (posicion > 0 && lista_cantidad(lista) <= posicion)
		return false;
	return true;
}

bool lista_insertar_posicion_0(lista_t *lista, void *elemento,
			       nodo_t *nuevo_nodo)
{
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = lista->primero;
	lista->primero = nuevo_nodo;

	lista->cantidad++;
	return true;
}

bool lista_insertar_en_medio(lista_t *lista, void *elemento, size_t posicion,
			     nodo_t *nuevo_nodo)
{
	nodo_t *nodo_previo = NULL;

	nodo_t *nodo_actual =
		buscando_nodo_por_pos(lista, posicion, &nodo_previo);
	if (!nodo_actual)
		return false;

	// Insierto el nuevo nodo
	nodo_previo->siguiente = nuevo_nodo;
	nuevo_nodo->elemento = elemento;
	nuevo_nodo->siguiente = nodo_actual;

	lista->cantidad++;
	return true;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (!validando_lista_y_posicion(lista, posicion))
		return false;

	nodo_t *nuevo_nodo = creando_nodo();
	if (!nuevo_nodo)
		return false;

	if (posicion == 0) {
		return lista_insertar_posicion_0(lista, elemento, nuevo_nodo);
	} else {
		return lista_insertar_en_medio(lista, elemento, posicion,
					       nuevo_nodo);
	}
}
//-----------------------------------------------
int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (!lista || !comparador || lista_vacia(lista))
		return -1;

	nodo_t *nodo_actual = lista->primero;
	int posicion = 0;

	while (nodo_actual) {
		if (comparador(nodo_actual->elemento, elemento) == 0) {
			return posicion; // elemento encontrado
		}
		nodo_actual = nodo_actual->siguiente;
		posicion++;
	}

	return -1; // no se encontró
}
//-----------------------------------------------
void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (!validando_lista_y_posicion(lista, posicion))
		return NULL;

	nodo_t *nodo = buscando_nodo_por_pos(lista, posicion, NULL);
	return nodo ? nodo->elemento : NULL;
}
//-----------------------------------------------
size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (!lista || !f || lista_vacia(lista))
		return 0; // no se procesó ningún elemento

	nodo_t *nodo_actual = lista->primero;
	size_t contador = 0;

	while (nodo_actual) {
		bool continuar = f(nodo_actual->elemento, extra);
		contador++;

		if (!continuar)
			break; // la función pidió parar

		nodo_actual = nodo_actual->siguiente;
	}

	return contador;
}
//-----------------------------------------------
void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (!validando_lista_y_posicion(lista, posicion))
		return NULL;

	nodo_t *nodo_previo = NULL;
	nodo_t *nodo_actual =
		buscando_nodo_por_pos(lista, posicion, &nodo_previo);
	if (!nodo_actual)
		return NULL;

	void *valor_aux = nodo_actual->elemento;

	if (nodo_previo) {
		// caso general: no es el primero
		nodo_previo->siguiente = nodo_actual->siguiente;
	} else {
		// caso especial: eliminamos el primero
		lista->primero = nodo_actual->siguiente;
	}

	//Caso que eliminamos el ultimo unico nodo
	if (!nodo_actual->siguiente)
		lista->ultimo = nodo_previo; //Es NULL

	free(nodo_actual);
	lista->cantidad--;
	return valor_aux;
}
//-----------------------------------------------
void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (!lista)
		return;

	nodo_t *nodo_actual = lista->primero;
	while (nodo_actual != NULL) {
		nodo_t *nodo_prox = nodo_actual->siguiente;
		if (destructor) {
			destructor(nodo_actual->elemento);
		}

		free(nodo_actual);
		nodo_actual = nodo_prox;
	}
	free(lista);
}
//-----------------------------------------------
void lista_destruir(lista_t *lista)
{
	lista_destruir_todo(lista, NULL);
}
//----------------------------------------------- Iterador externo
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	lista_iterador_t *it = calloc(1, sizeof(lista_iterador_t));
	if (!it)
		return NULL;

	it->actual = lista->primero;
	return it;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	if (!it)
		return false;

	return it->actual != NULL;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (!it || !it->actual)
		return;

	it->actual = it->actual->siguiente;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (!it || !it->actual)
		return NULL;

	return it->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	if (!it)
		return;

	free(it);
}
