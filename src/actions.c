#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "estructuras_de_datos/tp1.h"
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
	printf(ANSI_COLOR_WHITE ANSI_COLOR_BOLD
	       "=== POKÉMON ===" ANSI_COLOR_RESET "\n\n");

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Id:        " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%d\n" ANSI_COLOR_RESET, p->id);

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Nombre:    " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%s\n" ANSI_COLOR_RESET, p->nombre);

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Tipo:      " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%s\n" ANSI_COLOR_RESET,
	       convert_tipo_a_string(p->tipo));

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Ataque:    " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%d\n" ANSI_COLOR_RESET, p->ataque);

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Defensa:   " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%d\n" ANSI_COLOR_RESET, p->defensa);

	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD "Velocidad: " ANSI_COLOR_RESET);
	printf(ANSI_COLOR_DORADO "%d\n\n" ANSI_COLOR_RESET, p->velocidad);

	printf(ANSI_COLOR_BLACK "------------------------\n" ANSI_COLOR_RESET);

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

void imprimir_estilo(int estilo)
{
	switch (estilo) {
	case MENU_ESTILO_SIMPLE:
		printf(ANSI_COLOR_GREEN "Estilo cambiado a %sSIMPLE%s\n",
		       ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
		break;
	case MENU_ESTILO_COLORES:
		printf(ANSI_COLOR_GREEN "Estilo cambiado a %sCOLORES%s\n",
		       ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
		break;
	case MENU_ESTILO_COLORES2:
		printf(ANSI_COLOR_GREEN "Estilo cambiado a %sCOLORES 2%s\n",
		       ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
		break;
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}
//---------------------------------------------------------------------Actions----------------------------------------
void *buscar_por_nombre(void *contexto)
{
	tp1_t *tp1 = contexto;

	char *nombre_pk = calloc(LARGO_NOMBRE_POKEMON, sizeof(char));
	if (!nombre_pk)
		return NULL;

	printf(ANSI_COLOR_BLUE
	       "Ingrese el nombre de pokemon a buscar:\n" ANSI_COLOR_RESET);

	if (scanf(" %s", nombre_pk) != 1) {
		free(nombre_pk);
		return NULL;
	}

	struct pokemon *pk = tp1_buscar_nombre(tp1, nombre_pk);

	free(nombre_pk);

	if (!mostrar_pokemon(pk, NULL))
		printf(ANSI_COLOR_RED
		       "Pokemon no encontrado\n" ANSI_COLOR_RESET);

	return NULL;
}
void *buscar_por_id(void *contexto)
{
	tp1_t *tp1 = contexto;
	int id;

	printf(ANSI_COLOR_BLUE
	       "Ingrese el id de pokemon a buscar:\n" ANSI_COLOR_RESET);

	if (scanf(" %d", &id) != 1)
		return NULL;

	struct pokemon *pk = tp1_buscar_id(tp1, id);
	if (!mostrar_pokemon(pk, NULL))
		printf(ANSI_COLOR_RED
		       "Pokemon no encontrado\n" ANSI_COLOR_RESET);

	return NULL;
}
void *mostrar_por_nombre(void *contexto)
{
	tp1_t *tp1 = contexto;
	size_t cantidad_pk = tp1_cantidad(tp1);

	if (cantidad_pk == 0)
		return NULL;

	struct pokemon array_pk[cantidad_pk];

	struct contexto_pk cpk;
	cpk.array = array_pk;
	cpk.usados = 0;

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

	printf(ANSI_COLOR_BLUE
	       "Ingrese la ruta del archivo para cargarlo:\n" ANSI_COLOR_RESET);

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

bool carta_valida(size_t carta, size_t carta_ant, size_t cantidad)
{
	if (carta == 0 || carta > cantidad) {
		printf("%sValor fuera de rango. Debe estar entre 1 y %zu.%s\n",
		       ANSI_COLOR_RED, cantidad, ANSI_COLOR_RESET);
		return false;
	}

	if (carta == carta_ant) {
		printf("%sNo podes seleccionar la misma carta.%s\n",
		       ANSI_COLOR_RED, ANSI_COLOR_RESET);
		return false;
	}

	return true;
}
size_t pedir_carta_usuario(char *mensaje, juego_t *juego, size_t carta_ant)
{
	size_t pos_carta;
	size_t cantidad = juego_cartas_restantes(juego);

	while (true) {
		printf("%s%s%s\n", ANSI_COLOR_BLUE, mensaje, ANSI_COLOR_RESET);

		if (scanf(" %zu", &pos_carta) != 1) {
			// Limpiar buffer si el input no es un número
			int c;
			while ((c = getchar()) != '\n' && c != EOF)
				; // solo descartar los caracteres

			printf("%sValor ingresado incorrecto. Debe ser un número.%s\n",
			       ANSI_COLOR_RED, ANSI_COLOR_RESET);
			continue;
		}

		if (carta_valida(pos_carta, carta_ant, cantidad))
			break;
	}

	return pos_carta;
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

void imprimir_fila_cartas(lista_t *lista, size_t inicio, size_t fin)
{
	for (int linea = 0; linea < LINEAS; linea++) {
		for (size_t i = inicio; i < fin; i++) {
			switch (linea) {
			case 0:
				printf("┌────────────────┐");
				break;
			case 1:
			case 2:
			case 4:
			case 5:
				printf("│                │");
				break;
			case 3:
				printf("│       %02zu       │", i + 1);
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
void imprimir_cartas_en_fila_boca_abajo(lista_t *lista)
{
	size_t total = lista_cantidad(lista);
	if (total == 0)
		return;

	for (size_t inicio = 0; inicio < total; inicio += MAX_POR_FILA) {
		size_t fin = inicio + MAX_POR_FILA;
		if (fin > total)
			fin = total;

		imprimir_fila_cartas(lista, inicio, fin);
	}
}

void imprimir_linea_superior_jugadas(lista_t *lista, size_t inicio, size_t fin)
{
	for (size_t i = inicio; i < fin; i++) {
		jugada_registrada_t *jr = lista_buscar_elemento(lista, i);
		const char *color = jugada_encontrado(jr) ? ANSI_COLOR_GREEN :
							    ANSI_COLOR_RED;
		printf("%s┌──────┐   ┌──────┐%s   ", color, ANSI_COLOR_RESET);
	}
	printf("\n");
}
void imprimir_linea_central_jugadas(lista_t *lista, size_t inicio, size_t fin)
{
	for (size_t i = inicio; i < fin; i++) {
		jugada_registrada_t *jr = lista_buscar_elemento(lista, i);
		const char *color = jugada_encontrado(jr) ? ANSI_COLOR_GREEN :
							    ANSI_COLOR_RED;
		printf("%s│  %-2zu  │ → │  %-2zu  │%s   ", color,
		       jugada_carta_1(jr), jugada_carta_2(jr),
		       ANSI_COLOR_RESET);
	}
	printf("\n");
}
void imprimir_linea_inferior_jugadas(lista_t *lista, size_t inicio, size_t fin)
{
	for (size_t i = inicio; i < fin; i++) {
		jugada_registrada_t *jr = lista_buscar_elemento(lista, i);
		const char *color = jugada_encontrado(jr) ? ANSI_COLOR_GREEN :
							    ANSI_COLOR_RED;
		printf("%s└──────┘   └──────┘%s   ", color, ANSI_COLOR_RESET);
	}
	printf("\n\n");
}
void imprimir_jugadas_en_filas(lista_t *lista)
{
	size_t total = lista_cantidad(lista);
	if (total == 0) {
		printf("(sin jugadas)\n");
		return;
	}

	for (size_t inicio = 0; inicio < total; inicio += MAX_POR_FILA) {
		size_t fin = inicio + MAX_POR_FILA;
		if (fin > total)
			fin = total;

		imprimir_linea_superior_jugadas(lista, inicio, fin);
		imprimir_linea_central_jugadas(lista, inicio, fin);
		imprimir_linea_inferior_jugadas(lista, inicio, fin);
	}
}

void imprimir_encabezado_tablero()
{
	printf(ANSI_COLOR_BOLD ANSI_COLOR_BLACK
	       "========================================================\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
	       "              🃏 TABLERO DE JUEGO 🃏          \n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BLACK ANSI_COLOR_BOLD
	       "========================================================\n\n" ANSI_COLOR_RESET);
}
void imprimir_datos_jugadores(jugador_t *jugador1, jugador_t *jugador2,
			      jugador_t *jugador_actual)
{
	printf("Turno de : %s\n\n", jugador_obtener_nombre(jugador_actual));
	printf("Jugador 1: %s", jugador_obtener_nombre(jugador1));
	printf("		Jugador 2: %s\n",
	       jugador_obtener_nombre(jugador2));
	printf("Puntaje:   %zu", jugador_obtener_puntaje(jugador1));
	printf("		Puntaje:   %zu\n\n",
	       jugador_obtener_puntaje(jugador2));
}
void imprimir_ultimas_jugadas(lista_t *ultimas_jugadas)
{
	printf("---------------------------------------------------------\n");
	printf("                ÚLTIMAS %d JUGADAS\n", MAX_ULTIMAS_JUGADAS);
	printf("---------------------------------------------------------\n");

	if (lista_cantidad(ultimas_jugadas) == 0) {
		printf("No hay jugadas registradas aún.\n");
	} else {
		imprimir_jugadas_en_filas(ultimas_jugadas);
	}

	printf("\n=========================================\n\n");
}
void imprimir_tablero(juego_t *juego)
{
	if (!juego)
		return;

	jugador_t *jugador1 = juego_jugador_1(juego);
	jugador_t *jugador2 = juego_jugador_2(juego);
	jugador_t *jugador_actual = juego_jugador_actual(juego);
	lista_t *ultimas_jugadas = juego_ultimas_jugadas(juego);

	imprimir_encabezado_tablero();
	imprimir_datos_jugadores(jugador1, jugador2, jugador_actual);
	imprimir_ultimas_jugadas(ultimas_jugadas);
}

void imprimir_carta_boca_arriba(lista_t *l, size_t idx)
{
	struct pokemon *p = lista_buscar_elemento(l, idx - 1);

	printf("┌─────────────────┐\n");
	printf("│  %-13s  │\n", p->nombre);
	printf("│ Tipo:    %-4s   │\n", convert_tipo_a_string(p->tipo));
	printf("│                 │\n");
	printf("│ Ataque:     %-3d │\n", p->ataque);
	printf("│ Defensa:    %-3d │\n", p->defensa);
	printf("│ Velocidad:  %-3d │\n", p->velocidad);
	printf("└─────────────────┘\n");
}

void imprimir_encabezado_resultado()
{
	printf(ANSI_COLOR_BOLD ANSI_COLOR_BLACK
	       "========================================================\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
	       "                 ✨ RESULTADO FINAL ✨          \n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BOLD ANSI_COLOR_BLACK
	       "========================================================\n\n" ANSI_COLOR_RESET);
}
void imprimir_datos_jugador_final(jugador_t *jugador)
{
	lista_t *jugadas = jugador_registro_jugadas(jugador);
	size_t puntaje = jugador_obtener_puntaje(jugador);

	printf(ANSI_COLOR_BLUE "Jugador:%s %s\n", ANSI_COLOR_RESET,
	       jugador_obtener_nombre(jugador));
	printf(ANSI_COLOR_BLUE "Puntaje:%s %zu\n", ANSI_COLOR_RESET, puntaje);
	printf(ANSI_COLOR_BLUE "Jugadas:%s\n", ANSI_COLOR_RESET);
	imprimir_jugadas_en_filas(jugadas);
	printf("\n");
}
void imprimir_ganador(jugador_t *jugador1, jugador_t *jugador2)
{
	size_t puntaje1 = jugador_obtener_puntaje(jugador1);
	size_t puntaje2 = jugador_obtener_puntaje(jugador2);

	printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA
	       "🏆 Ganador 🏆\n" ANSI_COLOR_RESET);

	if (puntaje1 > puntaje2) {
		printf(ANSI_COLOR_BOLD ANSI_COLOR_DORADO
		       "EL CAMPEÓN ES: %s \n" ANSI_COLOR_RESET,
		       jugador_obtener_nombre(jugador1));
	} else if (puntaje2 > puntaje1) {
		printf(ANSI_COLOR_BOLD ANSI_COLOR_DORADO
		       "EL CAMPEÓN ES: %s \n" ANSI_COLOR_RESET,
		       jugador_obtener_nombre(jugador2));
	} else {
		printf(ANSI_COLOR_BOLD ANSI_COLOR_DORADO
		       "🤝 ¡EMPATE! Ambos jugadores dieron pelea.\n" ANSI_COLOR_RESET);
	}
}
void imprimir_resultado_final(juego_t *juego)
{
	if (!juego)
		return;

	jugador_t *jugador1 = juego_jugador_1(juego);
	jugador_t *jugador2 = juego_jugador_2(juego);

	imprimir_encabezado_resultado();
	imprimir_datos_jugador_final(jugador1);
	imprimir_datos_jugador_final(jugador2);
	imprimir_ganador(jugador1, jugador2);
}

void liberar_nombres(char *n1, char *n2)
{
	if (n1)
		free(n1);
	if (n2)
		free(n2);
}

void loop_juego(juego_t *juego, lista_t *juego_lista)
{
	while (!juego_terminado(juego)) {
		imprimir_cartas_en_fila_boca_abajo(juego_lista);

		imprimir_tablero(juego);

		size_t idx_1 = pedir_carta_usuario(
			"Elige la primera carta:", juego, 0);
		imprimir_carta_boca_arriba(juego_lista, idx_1);
		size_t idx_2 = pedir_carta_usuario(
			"Elige la segunda carta:", juego, idx_1);
		imprimir_carta_boca_arriba(juego_lista, idx_2);

		bool encontrada = juego_carta_encontrada(juego, idx_1, idx_2);
		juego_registrar_jugada(juego, idx_1, idx_2, encontrada);

		if (encontrada) {
			printf("%sCarta encontrada!%s\n", ANSI_COLOR_GREEN,
			       ANSI_COLOR_RESET);
			jugador_sumar_puntaje(juego_jugador_actual(juego));
		} else {
			printf("%sLas cartas no coinciden,turno del siguiente jugador%s\n",
			       ANSI_COLOR_RED, ANSI_COLOR_RESET);
			juego_siguiente_jugador(juego);
		}
	}
}

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
	liberar_nombres(nombre1, nombre2);
	if (!juego)
		return NULL;

	lista_t *juego_lista = juego_cartas_lista(juego);
	if (!juego_lista) {
		juego_destruir(juego);
		return NULL;
	}

	loop_juego(juego, juego_lista);

	imprimir_resultado_final(juego);

	juego_destruir(juego);

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

	int actual = menu_get_estilo(menus[0]);
	int siguiente = (actual + 1) % MENU_ESTILO_MAX;

	for (int i = 0; i < 3; i++) {
		menu_set_estilo(menus[i], siguiente);
	}

	imprimir_estilo(siguiente);

	return NULL;
}
void *menu_anterior(void *contexto)
{
	printf(ANSI_COLOR_BOLD ANSI_COLOR_BLACK
	       "Volviendo al menu anterior...\n" ANSI_COLOR_RESET);
	return NULL;
}
void *salir(void *contexto)
{
	printf(ANSI_COLOR_BOLD ANSI_COLOR_BLACK
	       "Saliendo del juego...\n" ANSI_COLOR_RESET);
	return NULL;
}