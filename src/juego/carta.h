#ifndef CARTA_H
#define CARTA_H

#include <stdbool.h>
#include "../estructuras_de_datos/tp1.h"

typedef struct carta carta_t;

// Crea una carta a partir de un Pokémon
carta_t *carta_crear(struct pokemon *pk);

// Devuelve si las cartas son iguales
bool cartas_son_iguales(carta_t *carta, carta_t *carta2);

// Marca la carta como encontrada
void carta_marcar_encontrada(carta_t *carta);

// Devuelve si la carta ya fue encontrada
bool carta_esta_encontrada(carta_t *carta);

// Libera la carta
void carta_destruir(void *);

// Devuelve el pokemon de la carta
struct pokemon *carta_datos_pk(carta_t *carta);

#endif // CARTA_H