#include "jugador.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct jugador {
	char *nombre;
	size_t puntaje;
	lista_t *registro_jugadas;
};

//------------------------------------------------------------------------------------------

bool jugador_reservar_nombre(jugador_t *jugador, char *nombre)
{
	size_t len = strlen(nombre);

	jugador->nombre = malloc(len + 1);
	if (!jugador->nombre)
		return false;

	strcpy(jugador->nombre, nombre);
	return true;
}

bool jugador_inicializar_lista(jugador_t *jugador)
{
	jugador->registro_jugadas = lista_crear();
	return jugador->registro_jugadas != NULL;
}

jugador_t *jugador_crear(char *nombre)
{
	if (!nombre)
		return NULL;

	jugador_t *jugador = calloc(1, sizeof(jugador_t));
	if (!jugador)
		return NULL;

	if (!jugador_reservar_nombre(jugador, nombre)) {
		jugador_destruir(jugador);
		return NULL;
	}

	if (!jugador_inicializar_lista(jugador)) {
		jugador_destruir(jugador);
		return NULL;
	}

	return jugador;
}
//------------------------------------------------------------------------------------------
bool jugador_registrar_jugada(jugador_t *jugador, jugada_registrada_t *jugada)
{
	if (!jugador || !jugada)
		return false;

	return lista_agregar(jugador->registro_jugadas, jugada);
}
//------------------------------------------------------------------------------------------
char *jugador_obtener_nombre(jugador_t *jugador)
{
	if (!jugador)
		return 0;

	return jugador->nombre;
}
//------------------------------------------------------------------------------------------
size_t jugador_obtener_puntaje(jugador_t *jugador)
{
	if (!jugador)
		return 0;

	return jugador->puntaje;
}
//------------------------------------------------------------------------------------------
void jugador_sumar_puntaje(jugador_t *jugador)
{
	if (!jugador)
		return;
	jugador->puntaje++;
}
//------------------------------------------------------------------------------------------
lista_t *jugador_registro_jugadas(jugador_t *jugador)
{
	if (!jugador)
		return NULL;

	return jugador->registro_jugadas;
}
//------------------------------------------------------------------------------------------

void jugador_destruir(jugador_t *jugador)
{
	if (jugador->nombre)
		free(jugador->nombre);
	if (jugador->registro_jugadas)
		lista_destruir(jugador->registro_jugadas);

	free(jugador);
}