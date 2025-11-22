#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "src/ansi.h"
#include "src/estructuras_de_datos/menu.h"
#include "src/estructuras_de_datos/tp1.h"
#include "src/constantes.h"
#include "src/actions.h"

struct contexto_jugar {
	tp1_t *tp1;
	char *nombre1;
	char *nombre2;
};

//-------------------------------------------Funciones aux-------------------------------------------------------------
char leer_tecla()
{
	int c;

	printf("%sIngrese la opción deseada:%s\n", ANSI_COLOR_BLUE,
	       ANSI_COLOR_RESET);

	do {
		c = getchar();
	} while (c == '\n' || c == '\r');

	c = toupper(c);

	return (char)c;
}

tp1_t *cargar_archivo_inicial(menu_t *menu, const char *ruta_archivo)
{
	tp1_t *tp1 = tp1_leer_archivo(ruta_archivo);

	if (tp1)
		printf("%sArchivo cargado exitosamente desde parámetros%s\n",
		       ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
	else
		printf("%sError cargando archivo desde parámetros%s\n",
		       ANSI_COLOR_RED, ANSI_COLOR_RESET);

	return tp1;
}

//-------------------------------------------Validaciones-------------------------------------------------------------
bool validando_params(int argc, char *argv[])
{
	if (argc > CANTIDAD_DE_PARAMETROS) {
		printf("%sSolo se pueden pasar 2 parametros <ejecutable> <archivo.csv>%s\n",
		       ANSI_COLOR_RED, ANSI_COLOR_RESET);
		return false;
	}
	return true;
}
bool validando_tp1(tp1_t *tp1, bool jugar)
{
	if (!tp1) {
		printf("%sDebe cargar un archivo pokemones primero%s\n",
		       ANSI_COLOR_DORADO, ANSI_COLOR_RESET);
		return false;
	}
	if (jugar) {
		size_t cantidad = tp1_cantidad(tp1);
		if (cantidad < 9) {
			printf("%sDebe de haber un minimo de 9 pokemones%s\n",
			       ANSI_COLOR_DORADO, ANSI_COLOR_RESET);
			return false;
		}
	}

	return true;
}
//-------------------------------------------Creando Menu------------------------------------------------------------
bool cargar_opciones_buscar(menu_t *submenu)
{
	return menu_agregar_opcion(submenu, 'N', "Buscar por nombre",
				   buscar_por_nombre) == 0 &&
	       menu_agregar_opcion(submenu, 'I', "Buscar por ID",
				   buscar_por_id) == 0 &&
	       menu_agregar_opcion(submenu, 'A', "Volver", menu_anterior) == 0;
}
bool cargar_opciones_mostrar(menu_t *submenu)
{
	return menu_agregar_opcion(submenu, 'N', "Mostrar por nombre",
				   mostrar_por_nombre) == 0 &&
	       menu_agregar_opcion(submenu, 'I', "Mostrar por ID",
				   mostrar_por_id) == 0 &&
	       menu_agregar_opcion(submenu, 'A', "Volver", menu_anterior) == 0;
}
bool cargar_opciones_principal(menu_t *menu, menu_t *sub_menu_buscar,
			       menu_t *sub_menu_mostrar)
{
	return menu_agregar_opcion(menu, 'C', "Cargar archivo",
				   cargar_archivo) == 0 &&
	       menu_agregar_submenu(menu, 'B', "Buscar", sub_menu_buscar) ==
		       0 &&
	       menu_agregar_submenu(menu, 'M', "Mostrar", sub_menu_mostrar) ==
		       0 &&
	       menu_agregar_opcion(menu, 'J', "Jugar", jugar) == 0 &&
	       menu_agregar_opcion(menu, 'S', "Jugar con semilla",
				   jugar_con_semilla) == 0 &&
	       menu_agregar_opcion(menu, 'E', "Cambiar estilo",
				   cambiar_estilo) == 0 &&
	       menu_agregar_opcion(menu, 'Q', "Salir del juego", salir) == 0;
}

bool creando_menu(menu_t **menu_principal, menu_t **sub_menu_buscar,
		  menu_t **sub_menu_mostrar)
{
	if (!menu_principal || !sub_menu_buscar || !sub_menu_mostrar)
		return false;

	*menu_principal = menu_crear("======= MENU TP2 =======");
	*sub_menu_buscar = menu_crear("|----- BUSCAR -----|");
	*sub_menu_mostrar = menu_crear("|----- MOSTRAR -----|");

	if (!*menu_principal || !*sub_menu_buscar || !*sub_menu_mostrar)
		return false;

	bool ok_buscar = cargar_opciones_buscar(*sub_menu_buscar);
	bool ok_mostrar = cargar_opciones_mostrar(*sub_menu_mostrar);
	bool ok_princ = cargar_opciones_principal(
		*menu_principal, *sub_menu_buscar, *sub_menu_mostrar);

	return ok_buscar && ok_mostrar && ok_princ;
}
//-------------------------------------------Juego-----------------------------------------------------------

//-------------------------------------------Cases Switch------------------------------------------------------------
tp1_t *case_cargar(menu_t *menu, char tecla)
{
	tp1_t *tp1 = menu_ejecutar(menu, tecla, NULL);
	if (tp1)
		printf("%sArchivo cargado exitosamente%s\n", ANSI_COLOR_GREEN,
		       ANSI_COLOR_RESET);
	else
		printf("%sError cargando archivo%s\n", ANSI_COLOR_RED,
		       ANSI_COLOR_RESET);
	return tp1;
}
void case_salir(menu_t *menu, char tecla, tp1_t *tp1, bool *continuar)
{
	*continuar = false;
	menu_ejecutar(menu, tecla, tp1);
}
void case_estilo(menu_t *menu, menu_t *sub_menu_buscar,
		 menu_t *sub_menu_mostrar, char tecla)
{
	menu_t *menus[CANT_MENUS];
	menus[0] = menu;
	menus[1] = sub_menu_buscar;
	menus[2] = sub_menu_mostrar;
	menu_ejecutar(menu, tecla, menus);
}
void case_jugar(menu_t *menu, char tecla, tp1_t *tp1)
{
	if (!validando_tp1(tp1, true))
		return;

	menu_ejecutar(menu, tecla, tp1);
	return;
}
//-------------------------------------------Switchs-----------------------------------------------------------------
void procesar_submenu(menu_t *submenu, tp1_t *tp1, const char *nombre)
{
	if (!validando_tp1(tp1, false))
		return;

	if (menu_mostrar(submenu) != 0) {
		printf("%sError mostrando menú %s%s\n", ANSI_COLOR_RED, nombre,
		       ANSI_COLOR_RESET);
		return;
	}

	char tecla = leer_tecla();

	switch (tecla) {
	case 'N':
	case 'I':
	case 'A':
		menu_ejecutar(submenu, tecla, tp1);
		break;
	default:
		printf("%sOpción inválida%s\n", ANSI_COLOR_RED,
		       ANSI_COLOR_RESET);
	}
}

void switch_principal(char tecla, menu_t *menu, menu_t *sub_menu_buscar,
		      menu_t *sub_menu_mostrar, tp1_t *tp1, bool *continuar)
{
	switch (tecla) {
	case 'C': // Cargar archivo
		tp1 = case_cargar(menu, tecla);
		break;

	case 'B': // Buscar
		procesar_submenu(sub_menu_buscar, tp1, "BUSCAR");
		break;

	case 'M': // Mostrar
		procesar_submenu(sub_menu_mostrar, tp1, "MOSTRAR");
		break;

	case 'J': // Jugar
		case_jugar(menu, tecla, tp1);
		break;
	case 'S': // Semilla
	case 'E': // Estilo
		case_estilo(menu, sub_menu_buscar, sub_menu_mostrar, tecla);
		break;

	case 'Q': // Quit
		case_salir(menu, tecla, tp1, continuar);
		break;

	default:
		printf("%sOpción inválida%s\n", ANSI_COLOR_RED,
		       ANSI_COLOR_RESET);
	}
}

void ejecutar_menu(menu_t *menu, menu_t *sub_menu_buscar,
		   menu_t *sub_menu_mostrar, char *ruta_archivo)
{
	tp1_t *tp1 = NULL;
	bool continuar = true;
	if (ruta_archivo)
		tp1 = cargar_archivo_inicial(menu, ruta_archivo);

	while (continuar) {
		if (menu_mostrar(menu) != 0) {
			printf("%sError mostrando menú principal%s\n",
			       ANSI_COLOR_RED, ANSI_COLOR_RESET);
			return;
		}

		char tecla = leer_tecla();

		switch_principal(tecla, menu, sub_menu_buscar, sub_menu_mostrar,
				 tp1, &continuar);
	}
}

//-------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (!validando_params(argc, argv))
		return TP2_ERROR_VALIDANDO_PARAMS;

	menu_t *menu = NULL;
	menu_t *sub_menu_buscar = NULL;
	menu_t *sub_menu_mostrar = NULL;
	char *ruta_archivo = NULL;

	if (!creando_menu(&menu, &sub_menu_buscar, &sub_menu_mostrar))
		return TP2_ERROR_CREANDO_MENU;

	if (argc == 2)
		ruta_archivo = argv[PARAMETRO_ARCHIVO_POKEDEX];

	ejecutar_menu(menu, sub_menu_buscar, sub_menu_mostrar, ruta_archivo);

	return TP2_EXITO;
}
