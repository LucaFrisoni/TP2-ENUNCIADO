#ifndef JUGADA_H
#define JUGADA_H

#include "carta.h"

typedef struct jugada_registrada jugada_registrada_t;

// Crea una nueva jugada
jugada_registrada_t *jugada_crear(carta_t *c1, carta_t *c2);

// Libera la jugada
void jugada_destruir(void *);

#endif