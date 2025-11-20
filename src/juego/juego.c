#include "juego.h"
#include "constantes.h"
#include "../estructuras_de_datos/lista.h"
#include "../estructuras_de_datos/hash.h"
#include <stdbool.h>

typedef struct {
	hash_t *cartas;
	size_t cartas_restantes;
	jugador_t *jugador1;
	jugador_t *jugador2;
	lista_t *ultimas_jugadas;
} juego_t;

//------------------------------------------------------------------------------------------
bool creando_jugadores(juego_t *juego, char *nombre1, char *nombre2)
{
	juego->jugador1 = jugador_crear(nombre1);
	juego->jugador2 = jugador_crear(nombre2);
	if (!juego->jugador1 || !juego->jugador2)
		return false;
	return true;
}

juego_t *juego_crear(tp1_t *tp1, char *nombre1, char *nombre2)
{
	if (!tp1 || !nombre1 || !nombre2)
		return NULL;

	juego_t *juego = calloc(1, sizeof(juego_t));
	if (!juego)
		return NULL;

	bool jugadores_creados = creando_jugadores(juego, nombre1, nombre2);
	if (!jugadores_creados)
		return NULL;
}
//------------------------------------------------------------------------------------------
void juego_siguiente_jugador(juego_t *juego)
{
}
//------------------------------------------------------------------------------------------
void juego_registrar_par(juego_t *juego, jugador_t *jugador, carta_t *c1,
			 carta_t *c2)
{
}
//------------------------------------------------------------------------------------------
void juego_mostrar_estado(const juego_t *juego)
{
}
//------------------------------------------------------------------------------------------
bool juego_terminado(const juego_t *juego)
{
}
//------------------------------------------------------------------------------------------
void juego_mostrar_resultado(const juego_t *juego)
{
}
//------------------------------------------------------------------------------------------
void juego_destruir(juego_t *juego)
{
}