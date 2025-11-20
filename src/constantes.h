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
	MENU_ERROR_DINAMICO = -5
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
#define LARGO_NOMBRE_ARCHIVO 100
#define CANT_MENUS 3

// #define ACTIONS_EXITO ((void *)0)
// #define ACTIONS_ERROR_DINAMICO ((void *)1)
// #define ACTIONS_ERROR_SCANF ((void *)2)

#endif /* CONSTANTES_H */