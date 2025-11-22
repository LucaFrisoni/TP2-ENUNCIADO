#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct menu menu_t;
typedef void *(*menu_action_t)(void *contexto);

typedef enum {
	MENU_ESTILO_SIMPLE,
	MENU_ESTILO_COLORES,
	MENU_ESTILO_COLORES2,
	MENU_ESTILO_MAX
} menu_estilo_t;

//Crea un menú vacío con un título y devuelve un puntero al menú.
menu_t *menu_crear(const char *titulo);

//Devuelve la cantidad del menu.
size_t menu_cantidad(menu_t *menu);

//Agrega una opción al menú: una tecla que el usuario puede presionar, un texto descriptivo y la función que se ejecutará.
//Debe de devolver 0 en caso de exito, un numero negativo en caso de error
int menu_agregar_opcion(menu_t *menu, char tecla, const char *descripcion,
			menu_action_t accion);

//Agrega un submenu asociado a una tecla y una descripción.
//Debe de devolver 0 en caso de exito, un numero negativo en caso de error
int menu_agregar_submenu(menu_t *menu, char tecla, const char *descripcion,
			 menu_t *submenu);

//Elimina una opción o submenu identificado por una tecla del menú.
//Debe de devolver 0 en caso de exito, un numero negativo en caso de error
int menu_eliminar_opcion(menu_t *menu, char tecla);

//Cambia el estilo visual del menú
//Debe de devolver 0 en caso de exito, un numero negativo en caso de error
int menu_set_estilo(menu_t *menu, menu_estilo_t estilo);

//Devuelve el estilo actual del menu
menu_estilo_t menu_get_estilo(menu_t *menu);

//Muestra el menu por pantalla y devuelve el valor booleano dependiendo si fueron mostradas todas las opciones del menu o no
//Debe de devolver 0 en caso de exito, un numero negativo en caso de error
int menu_mostrar(menu_t *menu);

//Muestra el menú, espera la entrada del usuario y ejecuta la acción o submenu correspondiente.
void *menu_ejecutar(menu_t *menu, char tecla, void *contexto);

//Libera toda la memoria usada por el menú y sus submenus.
void menu_destruir(menu_t *menu);

//Libera el menu,submenus y las opciones
void menu_destruir_todo(menu_t *menu);

#endif