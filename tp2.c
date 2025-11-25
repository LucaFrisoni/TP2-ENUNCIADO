#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "src/ansi.h"
#include "src/estructuras_de_datos/menu.h"
#include "src/estructuras_de_datos/opcion.h"
#include "src/estructuras_de_datos/tp1.h"
#include "src/estructuras_de_datos/lista.h"
#include "src/constantes.h"
#include "src/actions.h"

//-------------------------------------------Funciones aux-------------------------------------------------------------
char leer_tecla()
{
	char buffer[MAX_BUFFER];

	while (true) {
		printf("%sIngrese la opción deseada:%s\n", ANSI_COLOR_BLUE,
		       ANSI_COLOR_RESET);

		if (!fgets(buffer, sizeof(buffer), stdin)) {
			continue;
		}

		fflush(stdin);
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strlen(buffer) != 1) {
			printf("%sOpción inválida. Ingrese solo una letra.%s\n",
			       ANSI_COLOR_RED, ANSI_COLOR_RESET);
			continue;
		}

		return (char)toupper((unsigned char)buffer[0]);
	}
}

tp1_t *cargar_archivo_inicial(char *ruta_archivo)
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

void mostrar_opcion_con_estilo(menu_t *menu, opcion_t *op)
{
	switch (menu_get_estilo(menu)) {
	case MENU_ESTILO_SIMPLE:
		printf("- (%c) %s\n", opcion_tecla(op), opcion_descripcion(op));
		break;
	case MENU_ESTILO_COLORES:
		printf(ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
		       "- (%c)" ANSI_COLOR_RESET "  " ANSI_COLOR_CYAN
		       "%s" ANSI_COLOR_RESET "\n",
		       opcion_tecla(op), opcion_descripcion(op));
		break;
	case MENU_ESTILO_COLORES2:
		printf(ANSI_COLOR_RED "- (%c)" ANSI_COLOR_RESET
				      "  " ANSI_COLOR_WHITE ANSI_COLOR_BOLD
				      "%s" ANSI_COLOR_RESET "\n",
		       opcion_tecla(op), opcion_descripcion(op));
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}
void mostrar_titulo_con_estilo(menu_t *menu)
{
	switch (menu_get_estilo(menu)) {
	case MENU_ESTILO_SIMPLE:
		printf("%s\n", menu_get_titulo(menu));
		break;
	case MENU_ESTILO_COLORES:
		printf(ANSI_COLOR_WHITE ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET
							"\n",
		       menu_get_titulo(menu));
		break;
	case MENU_ESTILO_COLORES2:
		printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET
							  "\n",
		       menu_get_titulo(menu));
		break;
	case MENU_ESTILO_MAX:
	default:
		break;
	}
}
bool mostrar_opcion(void *elemento, void *contexto)
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

	lista_t *l = menu_get_opciones(menu);
	if (!l)
		return TP2_ERROR_MOSTRANDO_MENU;

	mostrar_titulo_con_estilo(menu);

	size_t n = menu_con_cada_opcion(menu, mostrar_opcion, menu);

	if (n != lista_cantidad(l))
		return TP2_ERROR_MOSTRANDO_MENU;

	return TP2_EXITO;
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

bool creando_menu(menu_t *menu_principal, menu_t *sub_menu_buscar,
		  menu_t *sub_menu_mostrar)
{
	if (!menu_principal || !sub_menu_buscar || !sub_menu_mostrar)
		return false;

	bool ok_buscar = cargar_opciones_buscar(sub_menu_buscar);
	bool ok_mostrar = cargar_opciones_mostrar(sub_menu_mostrar);
	bool ok_princ = cargar_opciones_principal(
		menu_principal, sub_menu_buscar, sub_menu_mostrar);

	return ok_buscar && ok_mostrar && ok_princ;
}
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
		   menu_t *sub_menu_mostrar, tp1_t *tp1)
{
	bool continuar = true;

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
//--------------------------------------------Destruccion-----------------------------------------------------------
void liberar_memoria(menu_t *menu_principal, tp1_t *tp1)
{
	menu_destruir(menu_principal);
	tp1_destruir(tp1);
}

//-------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (!validando_params(argc, argv))
		return TP2_ERROR_VALIDANDO_PARAMS;

	menu_t *menu = menu_crear("======= MENU TP2 =======");
	menu_t *sub_menu_buscar = menu_crear("|----- BUSCAR -----|");
	menu_t *sub_menu_mostrar = menu_crear("|----- MOSTRAR -----|");
	tp1_t *tp1 = NULL;

	if (!creando_menu(menu, sub_menu_buscar, sub_menu_mostrar))
		return TP2_ERROR_CREANDO_MENU;

	if (argc == 2)
		tp1 = cargar_archivo_inicial(argv[PARAMETRO_ARCHIVO_POKEDEX]);

	ejecutar_menu(menu, sub_menu_buscar, sub_menu_mostrar, tp1);
	liberar_memoria(menu, tp1);

	return TP2_EXITO;
}
