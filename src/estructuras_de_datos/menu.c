#include "menu.h"
#include "lista.h"
#include "hash.h"
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

typedef struct opcion {
	char tecla;
	char *descripcion;
	menu_action_t accion;
	menu_t *submenu;
} opcion_t;

//--------------------------------------------------------------------------------------------

char *copiar_string(const char *s)
{
	if (!s)
		return NULL;

	char *nuevo = malloc(strlen(s) + 1);
	if (!nuevo)
		return NULL;

	strcpy(nuevo, s);
	return nuevo;
}

opcion_t *creando_opcion(menu_t *submenu, char tecla, const char *descripcion,
			 menu_action_t accion)
{
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

menu_t *menu_crear(const char *titulo)
{
	if (!titulo)
		return NULL;

	menu_t *menu = calloc(1, sizeof(menu_t));
	if (!menu)
		return NULL;

	menu->titulo = copiar_string(titulo);
	if (!menu->titulo) {
		free(menu);
		return NULL;
	}

	menu->lista_opciones = lista_crear();
	if (!menu->lista_opciones) {
		free(menu->titulo);
		free(menu);
		return NULL;
	}

	menu->hash_opciones = hash_crear(DEFAULT_CAPACIDAD);
	if (!menu->hash_opciones) {
		free(menu->titulo);
		free(menu);
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
int menu_agregar(menu_t *menu, char tecla, const char *descripcion,
		 menu_action_t accion, menu_t *submenu)
{
	if (!menu || !descripcion)
		return MENU_ERROR_NULL;

	opcion_t *opcion = creando_opcion(submenu, tecla, descripcion, accion);
	if (!opcion)
		return MENU_ERROR_OPCION;

	if (!lista_agregar(menu->lista_opciones, opcion)) {
		free(opcion->descripcion);
		free(opcion);
		return MENU_ERROR_LISTA;
	}

	char *tecla_dinamica = calloc(2, sizeof(char));
	if (!tecla_dinamica) {
		free(opcion->descripcion);
		free(opcion);
		return MENU_ERROR_DINAMICO;
	}
	tecla_dinamica[0] = tecla;

	if (!hash_insertar(menu->hash_opciones, tecla_dinamica, opcion, NULL)) {
		lista_eliminar_elemento(menu->lista_opciones,
					lista_cantidad(menu->lista_opciones) -
						1);
		free(opcion->descripcion);
		free(opcion);
		return MENU_ERROR_HASH;
	}

	return MENU_EXITO;
}

int menu_agregar_opcion(menu_t *menu, char tecla, const char *descripcion,
			menu_action_t accion)
{
	if (!accion)
		return MENU_ERROR_NULL;

	return menu_agregar(menu, tecla, descripcion, accion, NULL);
}

int menu_agregar_submenu(menu_t *menu, char tecla, const char *descripcion,
			 menu_t *submenu)
{
	if (!submenu)
		return MENU_ERROR_NULL;

	return menu_agregar(menu, tecla, descripcion, NULL, submenu);
}
//--------------------------------------------------------------------------------------------
int comparar_opcion_por_tecla(const void *a, const void *b)
{
	const opcion_t *op1 = a;
	const char *tecla_buscada = b;

	if (!op1 || !tecla_buscada)
		return -1;

	return (op1->tecla == *tecla_buscada) ? 0 : 1;
}

int menu_eliminar_opcion(menu_t *menu, char tecla)
{
	if (!menu || !tecla)
		return MENU_ERROR_NULL;

	opcion_t *opcion_hash = hash_quitar(menu->hash_opciones, &tecla);
	if (!opcion_hash)
		return MENU_ERROR_HASH;

	int posicion = lista_buscar_posicion(menu->lista_opciones, &tecla,
					     comparar_opcion_por_tecla);

	if (posicion == -1) {
		free(opcion_hash->descripcion);
		free(opcion_hash);
		return MENU_ERROR_LISTA;
	}

	opcion_t *opcion_lista =
		lista_eliminar_elemento(menu->lista_opciones, (size_t)posicion);

	free(opcion_lista->descripcion);
	free(opcion_lista);

	return MENU_EXITO;
}
//--------------------------------------------------------------------------------------------
int menu_set_estilo(menu_t *menu, menu_estilo_t estilo)
{
	if (!menu)
		return MENU_ERROR_NULL;
	menu->estilo = estilo;
	return MENU_EXITO;
}

menu_estilo_t menu_get_estilo(menu_t *menu)
{
	if (!menu)
		return MENU_ERROR_NULL;
	return menu->estilo;
}
//--------------------------------------------------------------------------------------------
void mostrar_opcion_con_estilo(menu_t *menu, opcion_t *op)
{
	switch (menu->estilo) {
	case MENU_ESTILO_SIMPLE:
		printf("- (%c) %s\n", op->tecla, op->descripcion);
		break;
	case MENU_ESTILO_COLORES:
		printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "- (%c)" ANSI_COLOR_RESET "  " ANSI_COLOR_CYAN
		       "%s" ANSI_COLOR_RESET "\n",
		       op->tecla, op->descripcion);
		break;
	case MENU_ESTILO_COLORES2:
		printf(ANSI_COLOR_RED "- (%c)" ANSI_COLOR_RESET
				      "  " ANSI_COLOR_WHITE ANSI_COLOR_BOLD
				      "%s" ANSI_COLOR_RESET "\n",
		       op->tecla, op->descripcion);
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}
void mostrar_titulo_con_estilo(menu_t *menu)
{
	switch (menu->estilo) {
	case MENU_ESTILO_SIMPLE:
		printf("%s\n", menu->titulo);
		break;
	case MENU_ESTILO_COLORES:
		printf(ANSI_COLOR_WHITE ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET
							"\n",
		       menu->titulo);
		break;
	case MENU_ESTILO_COLORES2:
		printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET
							  "\n",
		       menu->titulo);
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}

bool mostrando_menu(void *elemento, void *contexto)
{
	opcion_t *op = elemento;
	menu_t *menu = contexto;

	if (!op || !menu)
		return false;

	mostrar_opcion_con_estilo(menu, op);
	return true;
}

int menu_mostrar(menu_t *menu)
{
	if (!menu)
		return MENU_ERROR_NULL;

	mostrar_titulo_con_estilo(menu);

	size_t n = lista_con_cada_elemento(menu->lista_opciones, mostrando_menu,
					   menu);

	if (n != lista_cantidad(menu->lista_opciones))
		return MENU_ERROR_LISTA;

	return MENU_EXITO;
}
//--------------------------------------------------------------------------------------------
void *menu_ejecutar(menu_t *menu, char tecla, void *contexto)
{
	if (!menu)
		return NULL;

	char *tecla_dinamica = calloc(2, sizeof(char));
	if (!tecla_dinamica)
		return NULL;

	tecla_dinamica[0] = tecla;

	opcion_t *opcion = hash_buscar(menu->hash_opciones, tecla_dinamica);

	free(tecla_dinamica);
	return opcion->accion(contexto);
}
//--------------------------------------------------------------------------------------------
void destructor_opcion(void *elemento)
{
	opcion_t *opcion = elemento;
	free(opcion->descripcion);
	free(opcion);
}

void menu_destruir_interno(menu_t *menu, bool destruir_opciones)
{
	if (!menu)
		return;

	if (destruir_opciones)
		lista_destruir_todo(menu->lista_opciones, destructor_opcion);
	else
		lista_destruir(menu->lista_opciones);

	hash_destruir(menu->hash_opciones);
	free(menu->titulo);
	free(menu);
}

void menu_destruir(menu_t *menu)
{
	menu_destruir_interno(menu, false);
}

void menu_destruir_todo(menu_t *menu)
{
	menu_destruir_interno(menu, true);
}
