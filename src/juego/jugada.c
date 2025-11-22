#include "jugada.h"
#include <stdlib.h>

struct jugada_registrada {
	carta_t *c1;
	carta_t *c2;
};

jugada_registrada_t *jugada_crear(carta_t *c1, carta_t *c2)
{
	jugada_registrada_t *j = malloc(sizeof(jugada_registrada_t));
	if (!j)
		return NULL;

	j->c1 = c1;
	j->c2 = c2;
	return j;
}

void jugada_destruir(void *jugada_void)
{
	jugada_registrada_t *jugada = jugada_void;
	free(jugada);
}