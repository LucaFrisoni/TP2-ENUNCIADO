#include "jugada.h"
#include <stdlib.h>
#include <stdbool.h>

struct jugada_registrada {
	size_t idx_p1;
	size_t idx_p2;
	bool encontrado;
};

jugada_registrada_t *jugada_crear(size_t idx_p1, size_t idx_p2, bool encontrado)
{
	jugada_registrada_t *j = malloc(sizeof(jugada_registrada_t));
	if (!j)
		return NULL;

	j->idx_p1 = idx_p1;
	j->idx_p2 = idx_p2;
	j->encontrado = encontrado;
	return j;
}

size_t jugada_carta_1(jugada_registrada_t *jugada)
{
	if (!jugada)
		return 0;
	return jugada->idx_p1;
}

size_t jugada_carta_2(jugada_registrada_t *jugada)
{
	if (!jugada)
		return 0;
	return jugada->idx_p2;
}

bool jugada_encontrado(jugada_registrada_t *jugada)
{
	if (!jugada)
		return false;
	return jugada->encontrado;
}

void jugada_destruir(void *jugada_void)
{
	jugada_registrada_t *jugada = jugada_void;
	free(jugada);
}
