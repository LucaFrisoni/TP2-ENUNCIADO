#include "juego.h"
#include "../estructuras_de_datos/lista.h"
#include <string.h>

typedef struct {
	char *nombre;
	size_t puntaje;
	lista_t *registro_jugadas;
} jugador_t;

//------------------------------------------------------------------------------------------
jugador_t *jugador_crear(char *nombre)
{
	if (!nombre)
		return NULL;

	jugador_t *jugador = calloc(1, sizeof(jugador_t));
	if (!jugador)
		return NULL;

	size_t len = strlen(nombre);
	jugador->nombre = malloc(len + 1);
	if (!jugador->nombre) {
		free(jugador);
		return NULL;
	}

	strcpy(jugador->nombre, nombre);

	return jugador;
}
//------------------------------------------------------------------------------------------
void jugador_registrar_jugada(jugador_t *jugador, carta_t *carta)
{
}
//------------------------------------------------------------------------------------------
size_t jugador_obtener_puntaje(jugador_t *jugador)
{
}
//------------------------------------------------------------------------------------------
void jugador_sumar_puntaje(jugador_t *jugador, size_t puntos)
{
}
//------------------------------------------------------------------------------------------
lista_t *jugador_registro_jugadas(jugador_t *jugador)
{
}
//------------------------------------------------------------------------------------------
void jugador_destruir(jugador_t *jugador)
{
}