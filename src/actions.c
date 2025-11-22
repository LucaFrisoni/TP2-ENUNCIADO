#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "estructuras_de_datos/tp1.h"
#include "estructuras_de_datos/cola.h"
#include "juego/juego.h"
#include "constantes.h"
#include "ansi.h"

struct contexto_pk {
	struct pokemon *array;
	size_t usados;
};

//-----------------------------------Funciones auxileares----------------------------------------

char *convert_tipo_a_string(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

bool mostrar_pokemon(struct pokemon *p, void *extra)
{
	if (!p)
		return false;
	printf("Id: %d\n", p->id);
	printf("Nombre: %s\n", p->nombre);
	printf("Tipo: %d\n", p->tipo);
	printf("Ataque: %d\n", p->ataque);
	printf("Defensa: %d\n", p->defensa);
	printf("Velocidad: %d\n\n", p->velocidad);

	(void)extra; // para evitar warnings de parámetro no usado
	return true;
}

void insertar_ordenado_nombre(struct pokemon *arr, size_t *usados,
			      struct pokemon *p)
{
	size_t i = *usados;

	arr[i] = *p;

	// burbujeo hacia atrás mientras el anterior sea "mayor"
	while (i > 0 && strcmp(arr[i - 1].nombre, arr[i].nombre) > 0) {
		struct pokemon tmp = arr[i];
		arr[i] = arr[i - 1];
		arr[i - 1] = tmp;
		i--;
	}

	(*usados)++;
}

bool ordenando_pokemon_por_nombre(struct pokemon *p, void *extra)
{
	struct contexto_pk *cpk = extra;
	insertar_ordenado_nombre(cpk->array, &cpk->usados, p);
	return true;
}

void imprimir_estilo(menu_estilo_t estilo)
{
	switch (estilo) {
	case MENU_ESTILO_SIMPLE:
		printf("Estilo cambiado a SIMPLE\n");
		break;
	case MENU_ESTILO_COLORES:
		printf("Estilo cambiado a COLORES\n");
		break;
	case MENU_ESTILO_COLORES2:
		printf("Estilo cambiado a COLORES 2\n");
		break;
	case MENU_ESTILO_RECUADRADO:
		printf("Estilo cambiado a RECUADRADO\n");
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}
//-----------------------------------Actions----------------------------------------
void *buscar_por_nombre(void *contexto)
{
	tp1_t *tp1 = contexto;

	char *nombre_pk = calloc(LARGO_NOMBRE_POKEMON, sizeof(char));
	if (!nombre_pk)
		return NULL;

	printf("Ingrese el nombre de pokemon a buscar:\n");

	if (scanf(" %s", nombre_pk) != 1) {
		free(nombre_pk);
		return NULL;
	}

	struct pokemon *pk = tp1_buscar_nombre(tp1, nombre_pk);

	free(nombre_pk);

	if (!mostrar_pokemon(pk, NULL))
		printf("Pokemon no encontrado\n");

	return NULL;
}
void *buscar_por_id(void *contexto)
{
	tp1_t *tp1 = contexto;
	int id;

	printf("Ingrese el id de pokemon a buscar:\n");

	if (scanf(" %d", &id) != 1)
		return NULL;

	struct pokemon *pk = tp1_buscar_id(tp1, id);
	if (!mostrar_pokemon(pk, NULL))
		printf("Pokemon no encontrado\n");

	return NULL;
}
void *mostrar_por_nombre(void *contexto)
{
	tp1_t *tp1 = contexto;
	size_t cantidad_pk = tp1_cantidad(tp1);

	if (cantidad_pk == 0)
		return NULL;

	struct pokemon array_pk[cantidad_pk];

	struct contexto_pk cpk = { .array = array_pk, .usados = 0 };

	tp1_con_cada_pokemon(tp1, ordenando_pokemon_por_nombre, &cpk);

	for (size_t i = 0; i < cpk.usados; i++)
		mostrar_pokemon(&array_pk[i], NULL);

	return NULL;
}
void *mostrar_por_id(void *contexto)
{
	tp1_t *tp1 = contexto;
	tp1_con_cada_pokemon(tp1, mostrar_pokemon, NULL);

	return NULL;
}
void *cargar_archivo(void *contexto)
{
	char *nombre_archivo = calloc(LARGO_NOMBRE_ARCHIVO, sizeof(char));
	if (!nombre_archivo)
		return NULL;

	printf("Ingrese la ruta del archivo para cargarlo:\n");

	if (scanf(" %s", nombre_archivo) != 1) {
		free(nombre_archivo);
		return NULL;
	}

	tp1_t *tp1 = tp1_leer_archivo(nombre_archivo);

	free(nombre_archivo);

	return tp1;
}
//-----------------------------------Juego----------------------------------------
char *pedir_string_usuario(char *mensaje)
{
	char *nombre = calloc(LARGO_NOMBRE_USUARIO, sizeof(char));
	if (!nombre)
		return NULL;

	printf("%s%s%s\n", ANSI_COLOR_BLUE, mensaje, ANSI_COLOR_RESET);

	if (scanf(" %s", nombre) != 1) {
		free(nombre);
		return NULL;
	}
	return nombre;
}

void imprimir_banner()
{
	printf("╔═══════════════════════════════════════════════════════════╗\n");
	printf("║ %s     ¡Bienvenido al Juego de Memoria!  %s %s Pokémon ⚡       %s║\n",
	       ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET, ANSI_COLOR_DORADO,
	       ANSI_COLOR_RESET);
	printf("║      %s 			Realizado por Luca Frisoni%s  ║\n",
	       ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
	printf("╚═══════════════════════════════════════════════════════════╝\033[0m\n\n");
}

void imprimir_cartas_en_fila_boca_abajo(lista_t *lista)
{
	size_t total = lista_cantidad(lista);
	if (total == 0)
		return;

	// 2) Imprimir por FILAS, no por columnas
	for (int inicio = 0; inicio < (int)total; inicio += CARTAS_POR_FILA) {
		int fin = inicio + CARTAS_POR_FILA;
		if (fin > (int)total)
			fin = (int)total;

		// imprimir las 7 líneas de esta fila
		for (int linea = 0; linea < LINEAS; linea++) {
			for (int i = inicio; i < fin; i++) {
				switch (linea) {
				case 0:
					printf("┌────────────────┐");
					break;
				case 1:
					printf("│                │");
					break;
				case 2:
					printf("│                │");
					break;
				case 3:
					printf("│       %02d       │", i + 1);
					break;
				case 4:
					printf("│                │");
					break;
				case 5:
					printf("│                │");
					break;
				case 6:
					printf("└────────────────┘");
					break;
				}

				printf("   "); // espacio entre cartas
			}

			printf("\n");
		}

		printf("\n"); // separar filas
	}
}

void imprimir_tablero(juego_t *juego)
{
	if (!juego)
		return;
	jugador_t *jugador1 = juego_jugador_1(juego);
	jugador_t *jugador2 = juego_jugador_1(juego);
	// lista_t *ultimas_jugadas = juego_ultimas_jugadas(juego);

	printf("\n=========================================\n");
	printf("               TABLERO DEL JUEGO\n");
	printf("=========================================\n\n");

	// Datos del jugador 1
	printf("Jugador 1: %s\n", jugador_obtener_nombre(jugador1));
	printf("Puntaje:   %zu\n\n", jugador_obtener_puntaje(jugador1));

	// Datos del jugador 2
	printf("Jugador 2: %s\n", jugador_obtener_nombre(jugador2));
	printf("Puntaje:   %zu\n\n", jugador_obtener_puntaje(jugador2));

	printf("-----------------------------------------\n");
	printf("          ÚLTIMAS %d JUGADAS\n", MAX_ULTIMAS_JUGADAS);
	printf("-----------------------------------------\n");

	// Mostrar jugadas
	// size_t total = lista_cantidad(ultimas_jugadas);
	// if (total == 0) {
	// 	printf("No hay jugadas registradas aún.\n");
	// 	return;
	// }

	// size_t inicio = 0;
	// if (total > MAX_ULTIMAS_JUGADAS)
	// 	inicio = total - MAX_ULTIMAS_JUGADAS;

	// for (size_t i = inicio; i < total; i++) {
	// 	char *jugada = cola_obtener(juego->jugadas, i);
	// 	if (jugada)
	// 		printf("- %s\n", jugada);
	// }

	printf("\n=========================================\n\n");
}
// void imprimir_cartas_en_fila_boca_arriba(lista_t *lista, struct pokemon *p)
// {
// 	size_t total = lista_cantidad(lista);
// 	if (total == 0)
// 		return;
// 	struct pokemon *p = carta_datos_pk(carta);

// 	// 2) Imprimir por FILAS, no por columnas
// 	for (int inicio = 0; inicio < (int)total; inicio += CARTAS_POR_FILA) {
// 		int fin = inicio + CARTAS_POR_FILA;
// 		if (fin > (int)total)
// 			fin = (int)total;

// 		// imprimir las 7 líneas de esta fila
// 		for (int linea = 0; linea < LINEAS; linea++) {
// 			for (int i = inicio; i < fin; i++) {
// 				switch (linea) {
// 				case 0:
// 					printf("┌────────────────┐");
// 					break;
// 				case 1:
// 					printf("│    %-9s   │", p->nombre);
// 					break;
// 				case 2:
// 					printf("│ Tipo:    %-4s  │",
// 					       convert_tipo_a_string(p->tipo));
// 					break;
// 				case 3:
// 					printf("│ Ataque:    %-3d │",
// 					       p->ataque);
// 					break;
// 				case 4:
// 					printf("│ Defensa:   %-3d │",
// 					       p->defensa);
// 					break;
// 				case 5:
// 					printf("│ Velocidad: %-3d │",
// 					       p->velocidad);
// 					break;
// 				case 6:
// 					printf("└────────────────┘");
// 					break;
// 				}

// 				printf("   "); // espacio entre cartas
// 			}

// 			printf("\n");
// 		}

// 		printf("\n"); // separar filas
// 	}
// }

void *jugar(void *contexto)
{
	tp1_t *tp1 = contexto;

	imprimir_banner();

	char *nombre1 =
		pedir_string_usuario("Ingrese el nombre del jugador 1:");
	char *nombre2 =
		pedir_string_usuario("Ingrese el nombre del jugador 2:");
	if (!nombre1 || !nombre2)
		return NULL;

	juego_t *juego = juego_crear(tp1, nombre1, nombre2);
	if (!juego)
		return NULL;

	lista_t *juego_lista = juego_cartas_lista(juego);
	if (!juego_lista) {
		return NULL;
	}

	imprimir_cartas_en_fila_boca_abajo(juego_lista);
	imprimir_tablero(juego);
	return NULL;
}
//-----------------------------------------------------------------------------------
void *jugar_con_semilla(void *contexto)
{
	return NULL;
}
void *cambiar_estilo(void *contexto)
{
	menu_t **menus = contexto;
	if (!menus)
		return NULL;

	menu_estilo_t actual = menu_get_estilo(menus[0]);
	menu_estilo_t siguiente = (actual + 1) % MENU_ESTILO_MAX;

	for (int i = 0; i < 3; i++) {
		menu_set_estilo(menus[i], siguiente);
	}

	imprimir_estilo(siguiente);

	return NULL;
}
void *menu_anterior(void *contexto)
{
	printf("Volviendo al menu anterior...\n");
	return NULL;
}
void *salir(void *contexto)
{
	printf("Saliendo del juego...\n");
	return NULL;
}