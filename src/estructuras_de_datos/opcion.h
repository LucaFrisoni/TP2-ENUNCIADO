#ifndef _OPCION_H_
#define _OPCION_H_

#include <stdio.h>
#include <stdbool.h>
#include "menu.h"

typedef struct opcion opcion_t;

// Crea una opcion para el menu asociada a una tecla junto a una copia de la explicacion sobre lo que realiza
// La opcion tambien puede ser un submenu
opcion_t *opcion_crear(menu_t *submenu, char tecla, const char *descripcion,
		       menu_action_t accion);

//Devuelve la tecla asociada a la opcion
char opcion_tecla(opcion_t *opcion);

//Devuelve la descripcion asociada a la opcion
char *opcion_descripcion(opcion_t *opcion);

//Devuelve la accion asociada a la opcion
menu_action_t opcion_accion(opcion_t *opcion);

//Libera la opcion
void opcion_destruir(void *opcion);

#endif