#ifndef JUGADOR_H
#define JUGADOR_H

#include "jugada.h"
#include "../estructuras_de_datos/lista.h"

typedef struct jugador jugador_t;

// Crea un jugador con un nombre
jugador_t *jugador_crear(char *nombre);

// Obtiene el nombre del jugador
char *jugador_obtener_nombre(jugador_t *jugador);

// Obtiene el puntaje del jugador
size_t jugador_obtener_puntaje(jugador_t *jugador);

// Incrementa puntaje del jugador
void jugador_sumar_puntaje(jugador_t *jugador);

// Registra una jugada
bool jugador_registrar_jugada(jugador_t *jugador, jugada_registrada_t *jugada);

// Devuelve el registro de jugadas del jugador
lista_t *jugador_registro_jugadas(jugador_t *jugador);

// Libera jugador
void jugador_destruir(jugador_t *jugador);

#endif // JUGADOR_H