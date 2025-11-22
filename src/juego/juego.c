#include "juego.h"
#include "../constantes.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct juego {
	hash_t *cartas;
	lista_t *lista_cartas;
	size_t cartas_restantes;
	jugador_t *jugador1;
	jugador_t *jugador2;
	jugador_t *jugador_actual;
	lista_t *ultimas_jugadas;
};

typedef struct contexto_cartas {
	hash_t *cartas;
	lista_t *lista_cartas;
	size_t total;
	size_t vistos;
	size_t insertados;
} contexto_cartas_t;

//------------------------------------------------------------------------------------------

void liberar_insercion_incompleta(carta_t *carta, carta_t *carta_copia,
				  char *clave2)
{
	if (carta)
		carta_destruir(carta);
	if (carta_copia)
		carta_destruir(carta_copia);
	if (clave2)
		free(clave2);
}

bool creando_jugadores(juego_t *juego, char *nombre1, char *nombre2)
{
	juego->jugador1 = jugador_crear(nombre1);
	juego->jugador2 = jugador_crear(nombre2);
	if (!juego->jugador1 || !juego->jugador2)
		return false;
	return true;
}

char *crear_clave_pokemon_duplicado(char *nombre)
{
	size_t len = strlen(nombre);
	char *clave = malloc(len + 2); // nombre + '2' + '\0'
	if (!clave)
		return NULL;

	strcpy(clave, nombre);
	clave[len] = '2';
	clave[len + 1] = '\0';

	return clave;
}

bool insertar_copia_doble(hash_t *h, lista_t *l, struct pokemon *p)
{
	carta_t *carta = carta_crear(p);
	carta_t *carta_copia = carta_crear(p);
	char *clave2 = crear_clave_pokemon_duplicado(p->nombre);
	if (!carta || !carta_copia || !clave2) {
		liberar_insercion_incompleta(carta, carta_copia, clave2);
		return false;
	}

	if (!hash_insertar(h, p->nombre, carta, NULL)) {
		liberar_insercion_incompleta(carta, carta_copia, clave2);
		return false;
	}
	if (!hash_insertar(h, clave2, carta_copia, NULL)) {
		liberar_insercion_incompleta(carta, carta_copia, clave2);
		return false;
	}
	if (!lista_agregar(l, carta)) {
		liberar_insercion_incompleta(carta, carta_copia, clave2);
		return false;
	}
	if (!lista_agregar(l, carta_copia)) {
		liberar_insercion_incompleta(carta, carta_copia, clave2);
		return false;
	}

	free(clave2);
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

	if (!insertar_copia_doble(c->cartas, c->lista_cartas, p))
		return false;

	c->insertados++;

	return true;
}

bool cargar_cartas_random(juego_t *juego, tp1_t *tp1)
{
	contexto_cartas_t ctx;

	ctx.cartas = juego->cartas;
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

	juego->cartas = hash_crear(DEFAULT_CAPACIDAD_JUEGO);
	if (!juego->cartas)
		return false;

	/* Seteo inicial de juego */
	juego->jugador_actual = juego->jugador1;
	juego->cartas_restantes = CANT_CARTAS_INICIALES;

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
bool registrar_ultimas_5_jugadas(juego_t *juego, jugada_registrada_t *jr)
{
	size_t ultima_jugadas = lista_cantidad(juego->ultimas_jugadas);
	if (!ultima_jugadas)
		return lista_agregar(juego->ultimas_jugadas, jr);

	if (ultima_jugadas >= MAX_ULTIMAS_JUGADAS)
		lista_eliminar_elemento(juego->ultimas_jugadas, 4);

	return lista_insertar(juego->ultimas_jugadas, jr, 0);
}

bool juego_registrar_par(juego_t *juego, carta_t *c1, carta_t *c2)
{
	if (!juego || !c1 || !c2)
		return false;

	if (cartas_son_iguales(c1, c2)) {
		carta_marcar_encontrada(c1);
		carta_marcar_encontrada(c2);
		jugador_sumar_puntaje(juego->jugador_actual);
		juego->cartas_restantes -= 2;
	}

	jugada_registrada_t *jr = jugada_crear(c1, c2);
	if (!jr)
		return false;

	if (!jugador_registrar_jugada(juego->jugador_actual, jr))
		return false;

	if (!registrar_ultimas_5_jugadas(juego, jr))
		return false;

	return true;
}
//------------------------------------------------------------------------------------------
bool juego_terminado(juego_t *juego)
{
	return (int)juego->cartas_restantes == 0;
}
//------------------------------------------------------------------------------------------
void *juego_cartas(juego_t *juego)
{
	if (!juego)
		return NULL;
	return juego->cartas;
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
	return juego->cartas_restantes;
}
//------------------------------------------------------------------------------------------
void juego_destruir_todo(juego_t *juego, bool destructor)
{
	if (!juego)
		return;

	if (juego->jugador1)
		jugador_destruir(juego->jugador1);
	if (juego->jugador2)
		jugador_destruir(juego->jugador2);

	if (juego->ultimas_jugadas) {
		if (destructor) {
			lista_destruir_todo(juego->ultimas_jugadas,
					    jugada_destruir);
		} else {
			lista_destruir(juego->ultimas_jugadas);
		}
	}

	if (juego->lista_cartas)
		lista_destruir(juego->lista_cartas);

	if (juego->cartas) {
		if (destructor) {
			hash_destruir_todo(juego->cartas, carta_destruir);
		} else {
			hash_destruir(juego->cartas);
		}
	}

	free(juego);
}

void juego_destruir(juego_t *juego)
{
	juego_destruir_todo(juego, false);
}
