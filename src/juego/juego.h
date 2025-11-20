#ifndef JUEGO_H
#define JUEGO_H

#include "carta.h"
#include "jugador.h"
#include "../estructuras_de_datos/tp1.h"
typedef struct juego juego_t;

// Crea un juego de cartas pokemon con dos jugadores
juego_t *juego_crear(tp1_t *tp1);

// Cambia de jugador
void juego_siguiente_jugador(juego_t *juego);

// Registra un par encontrado
void juego_registrar_par(juego_t *juego, jugador_t *jugador, carta_t *c1,
			 carta_t *c2);

// Muestra estado del juego: cartas disponibles + últimas jugadas
void juego_mostrar_estado(const juego_t *juego);

// Devuelve si aun siguen quedando cartas o no
bool juego_terminado(const juego_t *juego);

// Muestra el resultado final
void juego_mostrar_resultado(const juego_t *juego);

// Libera juego
void juego_destruir(juego_t *juego);

#endif // JUEGO_H