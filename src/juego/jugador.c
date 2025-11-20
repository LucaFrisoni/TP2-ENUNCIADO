#include "juego.h"
#include "../estructuras_de_datos/lista.h"

typedef struct {
	char *nombre;
	size_t puntaje;
	lista_t *registro_jugadas;
} jugador_t;

jugador_t *jugador_crear(const char *nombre)
{
}

void jugador_registrar_jugada(jugador_t *jugador, carta_t *carta)
{
}

size_t jugador_obtener_puntaje(const jugador_t *jugador)
{
}

void jugador_sumar_puntaje(jugador_t *jugador, size_t puntos)
{
}

lista_t *jugador_registro_jugadas(const jugador_t *jugador)
{
}

void jugador_destruir(jugador_t *jugador)
{
}