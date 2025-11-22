#ifndef JUGADA_H
#define JUGADA_H
#include <stdbool.h>
#include <stdio.h>

typedef struct jugada_registrada jugada_registrada_t;

// Crea una nueva jugada
jugada_registrada_t *jugada_crear(size_t idx_p1, size_t idx_p2,
				  bool encontrado);

// Libera la jugada
void jugada_destruir(void *);

//Devuelve la primera carta seleccionada de la jugada
size_t jugada_carta_1(jugada_registrada_t *jugada);

//Devuelve la segunda carta seleccionada de la jugada
size_t jugada_carta_2(jugada_registrada_t *jugada);

//Devuelve si en la jugada encontro el par
bool jugada_encontrado(jugada_registrada_t *jugada);

#endif