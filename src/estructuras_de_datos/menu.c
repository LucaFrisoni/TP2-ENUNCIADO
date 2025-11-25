#include "menu.h"
#include "lista.h"
#include "hash.h"
#include "opcion.h"
#include "../constantes.h"
#include "../ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct menu {
	char *titulo;
	hash_t *hash_opciones;
	lista_t *lista_opciones;
	menu_estilo_t estilo;
};

static char *copiar_string(char *s)
{
	if (!s)
		return NULL;

	char *nuevo = malloc(strlen(s) + 1);
	if (!nuevo)
		return NULL;

	strcpy(nuevo, s);
	return nuevo;
}

menu_t *menu_crear(char *titulo)
{
	if (!titulo)
		return NULL;

	menu_t *menu = calloc(1, sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->titulo = copiar_string(titulo);
	if (!menu->titulo) {
		menu_destruir(menu);
		return NULL;
	}

	menu->lista_opciones = lista_crear();
	if (!menu->lista_opciones) {
		menu_destruir(menu);
		return NULL;
	}

	menu->hash_opciones = hash_crear(DEFAULT_CAPACIDAD);
	if (!menu->hash_opciones) {
		menu_destruir(menu);
		return NULL;
	}

	return menu;
}
//--------------------------------------------------------------------------------------------
size_t menu_cantidad(menu_t *menu)
{
	if (!menu)
		return 0;
	return lista_cantidad(menu->lista_opciones);
}
//--------------------------------------------------------------------------------------------
int menu_agregar(menu_t *menu, char tecla, char *descripcion,
		 menu_action_t accion, menu_t *submenu)
{
	if (!menu || !descripcion)
		return MENU_ERROR_NULL;

	opcion_t *opcion = opcion_crear(submenu, tecla, descripcion, accion);
	if (!opcion)
		return MENU_ERROR_OPCION;

	if (!lista_agregar(menu->lista_opciones, opcion)) {
		opcion_destruir(opcion);
		return MENU_ERROR_LISTA;
	}

	char *tecla_dinamica = calloc(2, sizeof(char));
	if (!tecla_dinamica) {
		opcion_destruir(opcion);
		return MENU_ERROR_DINAMICO;
	}
	tecla_dinamica[0] = tecla;

	if (!hash_insertar(menu->hash_opciones, tecla_dinamica, opcion, NULL)) {
		lista_eliminar_elemento(menu->lista_opciones,
					lista_cantidad(menu->lista_opciones) -
						1);
		opcion_destruir(opcion);
		free(tecla_dinamica);
		return MENU_ERROR_HASH;
	}

	free(tecla_dinamica);
	return MENU_EXITO;
}

int menu_agregar_opcion(menu_t *menu, char tecla, char *descripcion,
			menu_action_t accion)
{
	if (!accion)
		return MENU_ERROR_NULL;

	return menu_agregar(menu, tecla, descripcion, accion, NULL);
}

int menu_agregar_submenu(menu_t *menu, char tecla, char *descripcion,
			 menu_t *submenu)
{
	if (!submenu)
		return MENU_ERROR_NULL;

	return menu_agregar(menu, tecla, descripcion, NULL, submenu);
}
//--------------------------------------------------------------------------------------------
int menu_set_estilo(menu_t *menu, menu_estilo_t estilo)
{
	if (!menu)
		return MENU_ERROR_NULL;
	if (estilo < 0 || estilo >= MENU_ESTILO_MAX)
		return MENU_ERROR_ESTILO_INVALIDO;
	menu->estilo = estilo;
	return MENU_EXITO;
}
//--------------------------------------------------------------------------------------------
int menu_get_estilo(menu_t *menu)
{
	if (!menu)
		return MENU_ERROR_NULL;
	return menu->estilo;
}
//--------------------------------------------------------------------------------------------
void *menu_get_opciones(menu_t *menu)
{
	if (!menu)
		return NULL;
	return menu->lista_opciones;
}
//--------------------------------------------------------------------------------------------
char *menu_get_titulo(menu_t *menu)
{
	if (!menu)
		return NULL;
	return menu->titulo;
}
//--------------------------------------------------------------------------------------------
size_t menu_con_cada_opcion(menu_t *menu,
			    bool (*funcion)(void *elemento, void *contexto),
			    void *contexto)
{
	if (!menu || !funcion)
		return 0;

	return lista_con_cada_elemento(menu->lista_opciones, funcion, contexto);
}
//--------------------------------------------------------------------------------------------
void *menu_ejecutar(menu_t *menu, char tecla, void *contexto)
{
	if (!menu)
		return NULL;

	char *tecla_dinamica;
	opcion_t *opcion;
	menu_action_t accion;

	tecla_dinamica = calloc(2, sizeof(char));
	if (!tecla_dinamica)
		return NULL;

	tecla_dinamica[0] = tecla;

	opcion = hash_buscar(menu->hash_opciones, tecla_dinamica);
	if (!opcion)
		return NULL;

	accion = opcion_accion(opcion);

	free(tecla_dinamica);
	return accion(contexto);
}
//--------------------------------------------------------------------------------------------
void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	if (menu->titulo)
		free(menu->titulo);
	if (menu->hash_opciones)
		hash_destruir(menu->hash_opciones);
	if (menu->lista_opciones)
		lista_destruir_todo(menu->lista_opciones, opcion_destruir);

	free(menu);
}
