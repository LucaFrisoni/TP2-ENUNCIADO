#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "estructuras_de_datos/tp1.h"
#include "constantes.h"

//-----------------------------------Funciones auxileares----------------------------------------
struct contexto_pk {
	struct pokemon *array;
	size_t usados;
};
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

	if (scanf(" %99s", nombre_pk) != 1) {
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
void *jugar(void *contexto)
{
	return NULL;
}
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