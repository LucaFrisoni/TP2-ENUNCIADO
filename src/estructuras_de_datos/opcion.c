#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

typedef struct opcion {
	char tecla;
	char *descripcion;
	menu_action_t accion;
	menu_t *submenu;
} opcion_t;

//---------------------------------------------------Aux
static char *copiar_string(const char *s)
{
	if (!s)
		return NULL;

	char *nuevo = malloc(strlen(s) + 1);
	if (!nuevo)
		return NULL;

	strcpy(nuevo, s);
	return nuevo;
}
//----------------------------------------------------------------------------------------------------------
opcion_t *opcion_crear(menu_t *submenu, char tecla, const char *descripcion,
		       menu_action_t accion)
{
	if (!tecla || !descripcion)
		return NULL;

	opcion_t *opcion = calloc(1, sizeof(opcion_t));
	if (!opcion)
		return NULL;

	opcion->tecla = tecla;
	opcion->descripcion = copiar_string(descripcion);
	if (!opcion->descripcion) {
		free(opcion);
		return NULL;
	}
	opcion->accion = accion;
	opcion->submenu = submenu;

	return opcion;
}
//----------------------------------------------------------------------------------------------------------
char opcion_tecla(opcion_t *opcion)
{
	return opcion->tecla;
}
//----------------------------------------------------------------------------------------------------------
char *opcion_descripcion(opcion_t *opcion)
{
	return opcion->descripcion;
}
//----------------------------------------------------------------------------------------------------------
menu_action_t opcion_accion(opcion_t *opcion)
{
	return opcion->accion;
}
//----------------------------------------------------------------------------------------------------------
void opcion_destruir(void *opcion)
{
	opcion_t *op = opcion;
	free(op->descripcion);
	free(op);
}