#ifndef JUEGO_H
#define JUEGO_H

#include "jugador.h"
#include "../estructuras_de_datos/hash.h"
#include "../estructuras_de_datos/tp1.h"

typedef struct juego juego_t;

// Crea un juego de cartas pokemon con dos jugadores
juego_t *juego_crear(tp1_t *tp1, char *nombre1, char *nombre2);

// Cambia el turno del  jugador
void juego_siguiente_jugador(juego_t *juego);

//Devuelve true si la carta fue encontrada
bool juego_carta_encontrada(juego_t *juego, size_t idx_p1, size_t idx_p2);

// Registra una jugada:
//Si la carta fue encontrada se la elimina
//Devuelve si la carta pudo ser registrada o no
bool juego_registrar_jugada(juego_t *juego, size_t idx_p1, size_t idx_p2,
			    bool encontrada);

//Devuelve si aun siguen quedando cartas o no
bool juego_terminado(juego_t *juego);

//Devuelve la cantidad de cartas en juego
size_t juego_cartas_restantes(juego_t *juego);

//Devuelve la estructura donde se guardan las cartas para mostrarlas
void *juego_cartas_lista(juego_t *juego);

//Devuelve la estructura donde se guardan las utlimas jugadas
void *juego_ultimas_jugadas(juego_t *juego);

//Devuelve el jugador 1
jugador_t *juego_jugador_1(juego_t *juego);

//Devuelve el jugador 2
jugador_t *juego_jugador_2(juego_t *juego);

//Devuelve el jugador actual
jugador_t *juego_jugador_actual(juego_t *juego);

// Libera juego
void juego_destruir(juego_t *juego);

#endif // JUEGO_H