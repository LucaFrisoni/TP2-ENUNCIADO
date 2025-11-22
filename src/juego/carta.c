#include "carta.h"
#include "../estructuras_de_datos/tp1.h"
#include <stdlib.h>
#include <stdbool.h>

struct carta {
	struct pokemon *p;
	bool encontrada;
};

//------------------------------------------------------------------------------------------
carta_t *carta_crear(struct pokemon *p)
{
	if (!p)
		return NULL;

	carta_t *carta = calloc(1, sizeof(carta_t));
	if (!carta)
		return NULL;

	carta->p = p;

	return carta;
}
//------------------------------------------------------------------------------------------
bool cartas_son_iguales(carta_t *carta, carta_t *carta2)
{
	if (!carta || carta2)
		return false;
	return carta->p->id == carta2->p->id;
}
//------------------------------------------------------------------------------------------
void carta_marcar_encontrada(carta_t *carta)
{
	if (!carta)
		return;
	carta->encontrada = true;
}
//------------------------------------------------------------------------------------------
bool carta_esta_encontrada(carta_t *carta)
{
	if (!carta)
		return false;
	return carta->encontrada;
}
//------------------------------------------------------------------------------------------
void carta_destruir(void *carta_void)
{
	carta_t *carta = carta_void;
	free(carta);
}
//------------------------------------------------------------------------------------------
struct pokemon *carta_datos_pk(carta_t *carta)
{
	if (!carta)
		return NULL;
	return carta->p;
}