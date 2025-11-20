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
void *liberar_juego_incompleto(juego_t *juego)
{
	if (juego->jugador1)
		jugador_destruir(juego->jugador1);
	if (juego->jugador2)
		jugador_destruir(juego->jugador2);
	if (juego->ultimas_jugadas)
		lista_destruir(juego->ultimas_jugadas);
	if (juego->cartas)
		hash_destruir(juego->cartas);

	free(juego);

	return NULL;
}

bool creando_jugadores(juego_t *juego, char *nombre1, char *nombre2)
{
	juego->jugador1 = jugador_crear(nombre1);
	juego->jugador2 = jugador_crear(nombre2);
	if (!juego->jugador1 || !juego->jugador2)
		return false;
	return true;
}

lista_t *creando_lista()
{
	return lista_crear();
}

hash_t *creando_hash()
{
	return hash_crear(DEFAULT_CAPACIDAD_JUEGO);
}

bool insertando_cartas_hash(struct pokemon *p, void *ctx)
{
	hash_t *cartas = ctx;
	if (!p || !cartas)
		return false;
	return hash_insertar(cartas, p->nombre, p, NULL);
}

// /* Selecciona 9 pokémon aleatorios del TP1 */
//  bool seleccionar_9_pokemon(tp1_t *tp1, struct pokemon *seleccionados[9])
// {
//     if (!tp1) return false;

//     size_t total = tp1_cantidad(tp1);
//     if (total < CANT_POKEMON) return false;

//     srand((unsigned)time(NULL));

//     for (int i = 0; i < CANT_POKEMON; i++) {

//         size_t idx = rand() % total;
//         seleccionados[i] = tp1_obtener(tp1, idx);

//         if (!seleccionados[i])
//             return false;
//     }

//     return true;
// }

// /* Inserta una carta (pokemon) al hash */
//  bool insertar_carta(hash_t *hash, struct pokemon *p, int id_duplicado)
// {
//     if (!hash || !p)
//         return false;

//     // Generamos clave única (ej: "Pikachu#1")
//     char clave[64];
//     snprintf(clave, sizeof(clave), "%s#%d", p->nombre, id_duplicado);

//     return hash_insertar(hash, clave, p, NULL);
// }

// /* Carga 9 pokemones elegidos y sus duplicados → 18 cartas */
//  bool cargar_cartas(juego_t *juego, tp1_t *tp1)
// {
//     struct pokemon *seleccionados[CANT_POKEMON] = {0};

//     if (!seleccionar_9_pokemon(tp1, seleccionados))
//         return false;

//     // Insertamos los 9 originales y 9 duplicados
//     for (int i = 0; i < CANT_POKEMON; i++) {

//         if (!insertar_carta(juego->cartas, seleccionados[i], 1))
//             return false;

//         if (!insertar_carta(juego->cartas, seleccionados[i], 2))
//             return false;
//     }

//     return true;
// }

juego_t *juego_crear(tp1_t *tp1, char *nombre1, char *nombre2)
{
	if (!tp1 || !nombre1 || !nombre2)
		return NULL;

	juego_t *juego = calloc(1, sizeof(juego_t));
	if (!juego)
		return NULL;

	bool jugadores_creados = creando_jugadores(juego, nombre1, nombre2);
	if (!jugadores_creados)
		return liberar_juego_incompleto(juego);

	juego->ultimas_jugadas = creando_lista();
	if (!juego->ultimas_jugadas)
		return liberar_juego_incompleto(juego);

	juego->cartas = creando_hash();
	if (!juego->cartas)
		return liberar_juego_incompleto(juego);

	tp1_con_cada_pokemon(tp1, insertando_cartas_hash, juego->cartas);
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