#ifndef JUGADOR_H
#define JUGADOR_H

#include "carta.h"

typedef struct jugador jugador_t;

// Crea un jugador con un nombre
jugador_t *jugador_crear(const char *nombre);

// Registra una jugada
void jugador_registrar_jugada(jugador_t *jugador, carta_t *carta);

// Obtiene el puntaje del jugador
size_t jugador_obtener_puntaje(const jugador_t *jugador);

// Incrementa puntaje del jugador
void jugador_sumar_puntaje(jugador_t *jugador, size_t puntos);

// Muestra las ultimas jugadas del juegador
void jugador_mostrar_ultimas_jugadas(const jugador_t *jugador);

// Libera jugador
void jugador_destruir(jugador_t *jugador);

#endif // JUGADOR_H