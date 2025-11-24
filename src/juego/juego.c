#include "juego.h"
#include "../constantes.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct juego {
	lista_t *lista_cartas;
	jugador_t *jugador1;
	jugador_t *jugador2;
	jugador_t *jugador_actual;
	lista_t *ultimas_jugadas;
};

typedef struct contexto_cartas {
	lista_t *lista_cartas;
	size_t total;
	size_t vistos;
	size_t insertados;
} contexto_cartas_t;

//------------------------------------------------------------------------------------------

bool creando_jugadores(juego_t *juego, char *nombre1, char *nombre2)
{
	juego->jugador1 = jugador_crear(nombre1);
	juego->jugador2 = jugador_crear(nombre2);
	if (!juego->jugador1 || !juego->jugador2)
		return false;
	return true;
}

bool insertar_doble(lista_t *l, struct pokemon *p)
{
	size_t cantidad = lista_cantidad(l);

	if (cantidad == 0) {
		if (!lista_agregar(l, p))
			return false;
		if (!lista_agregar(l, p))
			return false;
	} else {
		size_t pos1 = (size_t)(rand() % (int)cantidad);
		size_t pos2 = (size_t)(rand() % (int)cantidad);

		if (!lista_insertar(l, p, pos1))
			return false;
		if (!lista_insertar(l, p, pos2))
			return false;
	}

	return true;
}
bool debe_saltarse_insertar(contexto_cartas_t *c)
{
	size_t restantes = CANT_POKEMONES_CARTAS - c->insertados;
	size_t disponibles = c->total - (c->vistos - 1);

	double prob = (double)restantes / (double)disponibles;
	double r = (double)rand() / RAND_MAX;

	return (r > prob);
}
bool insertar_pokemon_random(struct pokemon *p, void *ctx)
{
	contexto_cartas_t *c = ctx;
	if (!c || !p)
		return false;

	c->vistos++;

	if (c->insertados == CANT_POKEMONES_CARTAS)
		return false;

	if (debe_saltarse_insertar(c))
		return true;

	if (!insertar_doble(c->lista_cartas, p))
		return false;

	c->insertados++;

	return true;
}
bool cargar_cartas_random(juego_t *juego, tp1_t *tp1)
{
	contexto_cartas_t ctx;

	ctx.lista_cartas = juego->lista_cartas;
	ctx.total = tp1_cantidad(tp1);
	ctx.vistos = 0;
	ctx.insertados = 0;

	tp1_con_cada_pokemon(tp1, insertar_pokemon_random, &ctx);

	if (ctx.insertados != CANT_POKEMONES_CARTAS)
		return false;

	return true;
}

bool creando_estructura_juego(juego_t *juego, char *nombre1, char *nombre2)
{
	if (!creando_jugadores(juego, nombre1, nombre2))
		return false;

	juego->ultimas_jugadas = lista_crear();
	juego->lista_cartas = lista_crear();
	if (!juego->ultimas_jugadas || !juego->lista_cartas)
		return false;

	juego->jugador_actual = juego->jugador1;

	return true;
}

juego_t *juego_crear(tp1_t *tp1, char *nombre1, char *nombre2)
{
	if (!tp1 || !nombre1 || !nombre2)
		return NULL;

	juego_t *juego = calloc(1, sizeof(juego_t));
	if (!juego)
		return NULL;

	if (!creando_estructura_juego(juego, nombre1, nombre2)) {
		juego_destruir(juego);
		return NULL;
	}

	if (!cargar_cartas_random(juego, tp1)) {
		juego_destruir(juego);
		return NULL;
	}

	return juego;
}
//------------------------------------------------------------------------------------------
void juego_siguiente_jugador(juego_t *juego)
{
	if (!juego)
		return;

	if (juego->jugador_actual == juego->jugador1) {
		juego->jugador_actual = juego->jugador2;
	} else {
		juego->jugador_actual = juego->jugador1;
	}
}
//------------------------------------------------------------------------------------------
jugador_t *juego_jugador_1(juego_t *juego)
{
	if (!juego)
		return NULL;

	return juego->jugador1;
}
//------------------------------------------------------------------------------------------
jugador_t *juego_jugador_2(juego_t *juego)
{
	if (!juego)
		return NULL;

	return juego->jugador2;
}
//------------------------------------------------------------------------------------------
jugador_t *juego_jugador_actual(juego_t *juego)
{
	if (!juego)
		return NULL;

	return juego->jugador_actual;
}
//------------------------------------------------------------------------------------------
bool juego_carta_encontrada(juego_t *juego, size_t idx_p1, size_t idx_p2)
{
	if (!juego)
		return false;
	struct pokemon *p1 =
		lista_buscar_elemento(juego->lista_cartas, idx_p1 - 1);
	struct pokemon *p2 =
		lista_buscar_elemento(juego->lista_cartas, idx_p2 - 1);
	if (!p1 || !p2)
		return false;
	return p1->id == p2->id;
}
//------------------------------------------------------------------------------------------
bool registrar_ultimas_5_jugadas(juego_t *juego, jugada_registrada_t *jr)
{
	size_t ultima_jugadas = lista_cantidad(juego->ultimas_jugadas);
	if (!ultima_jugadas)
		return lista_agregar(juego->ultimas_jugadas, jr);

	if (ultima_jugadas >= MAX_ULTIMAS_JUGADAS)
		lista_eliminar_elemento(juego->ultimas_jugadas, 4);

	return lista_insertar(juego->ultimas_jugadas, jr, 0);
}

bool juego_registrar_jugada(juego_t *juego, size_t idx_p1, size_t idx_p2,
			    bool encontrada)
{
	if (!juego)
		return false;

	jugada_registrada_t *jr = jugada_crear(idx_p1, idx_p2, encontrada);
	if (!jr)
		return false;

	if (!jugador_registrar_jugada(juego->jugador_actual, jr))
		return false;

	if (!registrar_ultimas_5_jugadas(juego, jr))
		return false;

	if (encontrada) {
		struct pokemon *p1 = lista_eliminar_elemento(
			juego->lista_cartas, idx_p1 - 1);
		struct pokemon *p2 = lista_eliminar_elemento(
			juego->lista_cartas, idx_p2 - 1);

		return p1 && p2;
	}

	return true;
}
//------------------------------------------------------------------------------------------
bool juego_terminado(juego_t *juego)
{
	return (int)lista_cantidad(juego->lista_cartas) == 0;
}
//------------------------------------------------------------------------------------------
void *juego_cartas_lista(juego_t *juego)
{
	if (!juego)
		return NULL;
	return juego->lista_cartas;
}
//------------------------------------------------------------------------------------------
void *juego_ultimas_jugadas(juego_t *juego)
{
	if (!juego)
		return NULL;
	return juego->ultimas_jugadas;
}
//------------------------------------------------------------------------------------------
size_t juego_cartas_restantes(juego_t *juego)
{
	if (!juego)
		return 0;
	return lista_cantidad(juego->lista_cartas);
}
//------------------------------------------------------------------------------------------
void juego_destruir(juego_t *juego)
{
	if (!juego)
		return;

	lista_t *jugadas_jg1 = jugador_registro_jugadas(juego->jugador1);
	lista_t *jugadas_jg2 = jugador_registro_jugadas(juego->jugador2);

	//Destruimos jugadas
	if (jugadas_jg1)
		lista_destruir_todo(jugadas_jg1, jugada_destruir);
	if (jugadas_jg2)
		lista_destruir_todo(jugadas_jg2, jugada_destruir);

	//	Destruimos jugador
	if (juego->jugador1)
		jugador_destruir(juego->jugador1);
	if (juego->jugador2)
		jugador_destruir(juego->jugador2);

	//Destruimos el juego
	if (juego->lista_cartas)
		lista_destruir(juego->lista_cartas);
	if (juego->ultimas_jugadas)
		lista_destruir(juego->ultimas_jugadas);

	free(juego);
}
