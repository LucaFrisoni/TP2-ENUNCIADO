#ifndef CONSTANTES_H
#define CONSTANTES_H

/* =======================================================
 *  Códigos de error del TP2
 * ======================================================= */

typedef enum {
	TP2_EXITO = 0,
	TP2_ERROR_NULL = -1,
	TP2_ERROR_VALIDANDO_PARAMS = -2,
	TP2_ERROR_CREANDO_MENU = -3,
	TP2_ERROR_DINAMICO = -4,
	// TP2_ERROR_SUBMENU_BUSCAR = -5,
	// TP2_ERROR_SUBMENU_MOSTRAR = -6,
	// TP2_ERROR_MOSTRANDO_MENU = -7,
	// TP2_ERROR_CASE_SALIR = -8,
} tp2_codigos_t;

/* =======================================================
 *  Códigos de error del módulo Menú
 * ======================================================= */

typedef enum {
	MENU_EXITO = 0,
	MENU_ERROR_NULL = -1,
	MENU_ERROR_OPCION = -2,
	MENU_ERROR_LISTA = -3,
	MENU_ERROR_HASH = -4,
	MENU_ERROR_DINAMICO = -5,
	MENU_ERROR_ESTILO_INVALIDO = -6
} menu_codigos_t;

/* =======================================================
 *  Códigos de error del actions
 * ======================================================= */

/* =======================================================
 *  Constantes varias del sistema
 * ======================================================= */

#define DEFAULT_CAPACIDAD 20
#define DEFAULT_CAPACIDAD_JUEGO 26
#define CANTIDAD_DE_PARAMETROS 2
#define PARAMETRO_ARCHIVO_POKEDEX 1
#define LARGO_NOMBRE_POKEMON 50
#define LARGO_NOMBRE_USUARIO 50
#define LARGO_NOMBRE_ARCHIVO 100
#define CANT_MENUS 3
#define CANT_CARTAS_INICIALES 18
#define CANT_POKEMONES_CARTAS 9
#define MAX_ULTIMAS_JUGADAS 5
#define MAX_POR_FILA 5
#define LINEAS 7
#define LINEAS_HISTORIAL 3
#define MAX_BUFFER 100

#endif /* CONSTANTES_H */