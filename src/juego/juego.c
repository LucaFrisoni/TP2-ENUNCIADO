#include "juego.h"
#include "constantes.h"
#include "../estructuras_de_datos/lista.h"
#include "../estructuras_de_datos/hash.h"

typedef struct {
	hash_t *cartas;
	size_t cartas_restantes;
	jugador_t *jugador1;
	jugador_t *jugador2;
	lista_t *ultimas_jugadas;
} juego_t;

juego_t *juego_crear(tp1_t *tp1)
{
}

void juego_siguiente_jugador(juego_t *juego)
{
}

void juego_registrar_par(juego_t *juego, jugador_t *jugador, carta_t *c1,
			 carta_t *c2)
{
}

void juego_mostrar_estado(const juego_t *juego)
{
}

bool juego_terminado(const juego_t *juego)
{
}

void juego_mostrar_resultado(const juego_t *juego)
{
}

void juego_destruir(juego_t *juego)
{
}