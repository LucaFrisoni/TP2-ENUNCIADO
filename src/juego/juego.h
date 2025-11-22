#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "jugador.h"
#include "../estructuras_de_datos/hash.h"

typedef struct juego juego_t;

// Crea un juego de cartas pokemon con dos jugadores
juego_t *juego_crear(tp1_t *tp1, char *nombre1, char *nombre2);

// Cambia de jugador
void juego_siguiente_jugador(juego_t *juego);

// Registra un par encontrado
bool juego_registrar_par(juego_t *juego, carta_t *c1, carta_t *c2);

// Devuelve si aun siguen quedando cartas o no
bool juego_terminado(juego_t *juego);

// Devuelve la cantidad de cartas en juego
size_t juego_cartas_restantes(juego_t *juego);

// Devuelve la estructura donde se guardan las cartas para buscar
void *juego_cartas(juego_t *juego);

// Devuelve la estructura donde se guardan las cartas para mostrarlas
void *juego_cartas_lista(juego_t *juego);

//Devuelve el jugador 1
jugador_t *juego_jugador_1(juego_t *juego);

//Devuelve el jugador 2
jugador_t *juego_jugador_2(juego_t *juego);

//Devuelve el jugador actual
jugador_t *juego_jugador_actual(juego_t *juego);

// Devuelve la estructura donde se guardan las utlimas jugadas
void *juego_ultimas_jugadas(juego_t *juego);

// Libera juego
void juego_destruir(juego_t *juego);

// Libera el juego y todos sus elementos
void juego_destruir_todo(juego_t *juego, bool destructor);

#endif // JUEGO_H