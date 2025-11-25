#include "pa2m.h"
#include "src/estructuras_de_datos/menu.h"
#include "src/estructuras_de_datos/opcion.h"
#include "src/juego/juego.h"
#include "src/juego/jugador.h"
#include "src/juego/jugada.h"
#include "src/estructuras_de_datos/lista.h"
#include "string.h"

struct struct_prueba {
	int x;
	double y;
};
//--------------------------------------------------MENU-------------------------

void test_menu_crear_titulo_null()
{
	menu_t *menu = menu_crear(NULL);
	pa2m_afirmar(menu == NULL,
		     "Crear un menu con un titulo NULL devuelve NULL");
}
void test_menu_crear_correctamente()
{
	menu_t *menu = menu_crear("Menu tp2");
	pa2m_afirmar(menu != NULL,
		     "Crear un menu con un titulo lo crea correctamente");

	menu_destruir(menu);
}
void test_menu_crear_titulo_largo()
{
	menu_t *menu = menu_crear(
		"Titulo largoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
	pa2m_afirmar(menu != NULL,
		     "Crear un menu con un titulo largo lo crea correctamente");
	menu_destruir(menu);
}
void test_menu_crear_titulo_una_letra()
{
	menu_t *menu = menu_crear("A");
	pa2m_afirmar(
		menu != NULL,
		"Crear un menu con un titulo de una letra lo crea correctamente");
	menu_destruir(menu);
}
void test_menu_crear_titulo_emojis()
{
	menu_t *menu = menu_crear("Pokémon – Menú ✨🔥");
	pa2m_afirmar(
		menu != NULL,
		"Se puede crear un menú con un título con emoji y unicode.");

	menu_destruir(menu);
}
void test_menu_crear_titulo_simbolos()
{
	menu_t *menu = menu_crear("##@@!!??");
	pa2m_afirmar(menu != NULL,
		     "Se puede crear un menú con símbolos especiales.");

	menu_destruir(menu);
}

void tests_menu_crear()
{
	test_menu_crear_titulo_null();
	test_menu_crear_correctamente();
	test_menu_crear_titulo_largo();
	test_menu_crear_titulo_una_letra();
	test_menu_crear_titulo_emojis();
	test_menu_crear_titulo_simbolos();
}
//-------------------------------------------------
void test_menu_cantidad_menu_null()
{
	size_t cantidad = menu_cantidad(NULL);
	pa2m_afirmar(cantidad == 0, "Pasarle NULL a cantidad devuelve 0");
}
void test_menu_cantidad_al_crear()
{
	menu_t *menu = menu_crear("Menu");
	size_t cantidad = menu_cantidad(menu);
	pa2m_afirmar(cantidad == 0,
		     "Un menu recien creado inicializa con cantidad 0");

	menu_destruir(menu);
}

void tests_menu_cantidad()
{
	test_menu_cantidad_menu_null();
	test_menu_cantidad_al_crear();
}
//-------------------------------------------------
void *accion_int(void *c)
{
	static int v = 7;
	return &v; // devolvemos puntero al static
}
void *accion_double(void *c)
{
	static double d = 3.14;
	return &d;
}
void *accion_bool(void *c)
{
	static bool b = true;
	return &b;
}
void *accion_char(void *c)
{
	static char x = 'Z';
	return &x;
}
void *accion_string(void *c)
{
	return "Hola";
}
void *accion_array(void *c)
{
	static int arr[3] = { 1, 2, 3 };
	return arr;
}
void *accion_struct(void *c)
{
	static struct struct_prueba d = { 10, 2.5 };
	return &d;
}
void *accion_null(void *c)
{
	return NULL;
}
void *accion_con_contexto(void *ctx)
{
	static int retorno;
	int *valor = ctx;
	retorno = (*valor) * 2;
	return &retorno;
}

void test_menu_agregar_opcion_menu_null()
{
	int resultado =
		menu_agregar_opcion(NULL, 'a', "Opcion", (menu_action_t)1);
	pa2m_afirmar(resultado == -1,
		     "Agregar opcion a menu NULL devuelve error NULL");
}
void test_menu_agregar_opcion_descripcion_null()
{
	menu_t *menu = menu_crear("Menu");
	int resultado = menu_agregar_opcion(menu, 'a', NULL, (menu_action_t)1);
	pa2m_afirmar(resultado == -1,
		     "Agregar opcion con descripcion NULL devuelve error NULL");
	menu_destruir(menu);
}
void test_menu_agregar_opcion_accion_null()
{
	menu_t *menu = menu_crear("Menu");
	int resultado = menu_agregar_opcion(menu, 'a', "Opcion", NULL);
	pa2m_afirmar(resultado == -1,
		     "Agregar opcion con accion NULL devuelve error NULL");
	menu_destruir(menu);
}
void test_menu_agregar_opcion_correctamente()
{
	menu_t *menu = menu_crear("Menu");
	int resultado =
		menu_agregar_opcion(menu, 'a', "Opcion A", (menu_action_t)1);

	pa2m_afirmar(resultado == 0,
		     "Agregar opcion simple correctamente devuelve exito");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_correctamente_varias_opciones()
{
	menu_t *menu = menu_crear("Menu");

	menu_agregar_opcion(menu, 'a', "A", (menu_action_t)1);
	menu_agregar_opcion(menu, 'b', "B", (menu_action_t)1);
	menu_agregar_opcion(menu, 'c', "C", (menu_action_t)1);

	pa2m_afirmar(menu_cantidad(menu) == 3,
		     "Agregar varias opciones funciona correctamente");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_aumenta_cantidad()
{
	menu_t *menu = menu_crear("Menu");

	size_t antes = menu_cantidad(menu);
	menu_agregar_opcion(menu, 'x', "Hola", (menu_action_t)1);
	size_t despues = menu_cantidad(menu);

	pa2m_afirmar(despues == antes + 1,
		     "Agregar una opcion incrementa la cantidad");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_descripciones_duplicadas()
{
	menu_t *m = menu_crear("Menu");

	int r1 = menu_agregar_opcion(m, 'a', "Desc", (menu_action_t)1);
	int r2 = menu_agregar_opcion(m, 'b', "Desc", (menu_action_t)1);

	pa2m_afirmar(
		r1 == 0 && r2 == 0,
		"Se pueden agregar opciones con la misma descripción pero distinta tecla");

	menu_destruir(m);
}
void test_menu_agregar_opcion_descripcion_larga()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_opcion(
		menu, 'a',
		"Descripcion muy muy largaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa para probar comportamiento del TDA",
		(menu_action_t)1);

	pa2m_afirmar(r == 0, "Agregar opcion con descripcion larga funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_descripcion_corta()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_opcion(menu, 'a', "X", (menu_action_t)1);
	pa2m_afirmar(r == 0,
		     "Agregar opcion con descripcion de 1 letra funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_descripcion_emojis()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_opcion(menu, 'a', "✨🔥Opción🍀",
				    (menu_action_t)1);
	pa2m_afirmar(r == 0, "Agregar opcion con emojis y unicode funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_descripcion_simbolos()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_opcion(menu, 'a', "#@!!$%&", (menu_action_t)1);
	pa2m_afirmar(r == 0, "Agregar opcion con símbolos especiales funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_accion_devuelve_varios_tipos_datos()
{
	menu_t *menu = menu_crear("Menu");

	pa2m_afirmar(
		menu_agregar_opcion(menu, '1', "int", accion_int) == 0,
		"Agregar opcion con acción que devuelve int* puede agregarse");
	pa2m_afirmar(
		menu_agregar_opcion(menu, '2', "double", accion_double) == 0,
		"Agregar opcion con acción que devuelve double* puede agregarse");
	pa2m_afirmar(
		menu_agregar_opcion(menu, '3', "bool", accion_bool) == 0,
		"Agregar opcion con acción que devuelve bool* puede agregarse");
	pa2m_afirmar(
		menu_agregar_opcion(menu, '4', "char", accion_char) == 0,
		"Agregar opcion con acción que devuelve char* puede agregarse");
	pa2m_afirmar(
		menu_agregar_opcion(menu, '5', "string", accion_string) == 0,
		"Agregar opcion con acción que devuelve string puede agregarse");
	pa2m_afirmar(menu_agregar_opcion(menu, '6', "array", accion_array) == 0,
		     "Agregar opcion con acción que devuelve array funciona");
	pa2m_afirmar(menu_agregar_opcion(menu, '7', "struct", accion_struct) ==
			     0,
		     "Agregar opcion con acción que devuelve struct* funciona");
	pa2m_afirmar(menu_agregar_opcion(menu, '8', "null", accion_null) == 0,
		     "Agregar opcion con acción que devuelve NULL funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_accion_pasandole_contexto()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_opcion(menu, 'x', "Con contexto",
				    accion_con_contexto);
	pa2m_afirmar(r == 0,
		     "Agregar opcion con accion que usa contexto funciona");

	menu_destruir(menu);
}
void test_menu_agregar_opcion_accion_prueba_estress()
{
	menu_t *menu = menu_crear("Stress");

	bool ok = true;
	for (int i = 0; i < 5000; i++) {
		char tecla = (char)((i % 26) + 'a');
		if (menu_agregar_opcion(menu, tecla, "Opcion",
					(menu_action_t)1) != 0) {
			ok = false;
			break;
		}
	}

	pa2m_afirmar(
		ok,
		"Prueba estres:Agregar 5000 opciones funciona correctamente");

	menu_destruir(menu);
}

void tests_menu_agregar_opcion()
{
	test_menu_agregar_opcion_menu_null();
	test_menu_agregar_opcion_descripcion_null();
	test_menu_agregar_opcion_accion_null();
	test_menu_agregar_opcion_correctamente();
	test_menu_agregar_opcion_correctamente_varias_opciones();
	test_menu_agregar_opcion_aumenta_cantidad();
	test_menu_agregar_opcion_descripciones_duplicadas();
	test_menu_agregar_opcion_descripcion_larga();
	test_menu_agregar_opcion_descripcion_corta();
	test_menu_agregar_opcion_descripcion_emojis();
	test_menu_agregar_opcion_descripcion_simbolos();
	test_menu_agregar_opcion_accion_devuelve_varios_tipos_datos();
	test_menu_agregar_opcion_accion_pasandole_contexto();
	test_menu_agregar_opcion_accion_prueba_estress();
}
//-------------------------------------------------
void test_menu_agregar_submenu_menu_null()
{
	int r = menu_agregar_submenu(NULL, 'a', "Sub", (menu_t *)1);
	pa2m_afirmar(r == -1,
		     "Agregar submenu con menu NULL devuelve error NULL");
}
void test_menu_agregar_submenu_descripcion_null()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Sub");

	int r = menu_agregar_submenu(menu, 'a', NULL, sub);

	pa2m_afirmar(
		r == -1,
		"Agregar submenu con descripción NULL devuelve error null");

	menu_destruir(sub);
	menu_destruir(menu);
}
void test_menu_agregar_submenu_submenu_null()
{
	menu_t *menu = menu_crear("Menu");

	int r = menu_agregar_submenu(menu, 'a', "Submenu", NULL);
	pa2m_afirmar(r == -1, "Agregar submenu NULL devuelve error null");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_correctamente()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Submenu");

	int r = menu_agregar_submenu(menu, 'a', "Ir al submenu", sub);
	pa2m_afirmar(r == 0, "Agregar un submenu correctamente devuelve exito");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_correctamente_varios_submenus()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *s1 = menu_crear("S1");
	menu_t *s2 = menu_crear("S2");
	menu_t *s3 = menu_crear("S3");

	int r1 = menu_agregar_submenu(menu, 'a', "Uno", s1);
	int r2 = menu_agregar_submenu(menu, 'b', "Dos", s2);
	int r3 = menu_agregar_submenu(menu, 'c', "Tres", s3);

	pa2m_afirmar(r1 == 0 && r2 == 0 && r3 == 0,
		     "Agregar varios submenus funciona correctamente");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_aumenta_cantidad()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Submenu");

	size_t antes = menu_cantidad(menu);
	menu_agregar_submenu(menu, 'x', "Sub", sub);
	size_t despues = menu_cantidad(menu);

	pa2m_afirmar(despues == antes + 1,
		     "Agregar un submenu incrementa la cantidad");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_descripcion_larga()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Sub");

	int r = menu_agregar_submenu(
		menu, 'a',
		"Descripcion largaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
		sub);

	pa2m_afirmar(
		r == 0,
		"Agregar submenu con descripción larga funciona correctamente");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_descripcion_corta()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Sub");

	int r = menu_agregar_submenu(menu, 'a', "X", sub);
	pa2m_afirmar(r == 0,
		     "Agregar submenu con descripción de una letra funciona");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_descripcion_emojis()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Sub");

	int r = menu_agregar_submenu(menu, 'a', "✨🔥🌙", sub);
	pa2m_afirmar(r == 0, "Agregar submenu con descripción emoji funciona");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_descripcion_simbolos()
{
	menu_t *menu = menu_crear("Menu");
	menu_t *sub = menu_crear("Sub");

	int r = menu_agregar_submenu(menu, 'a', "@@@###%%%!!!", sub);
	pa2m_afirmar(r == 0, "Agregar submenu con símbolos funciona");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_descripciones_iguales()
{
	menu_t *m = menu_crear("Menu");
	menu_t *s1 = menu_crear("S1");
	menu_t *s2 = menu_crear("S2");

	int r1 = menu_agregar_submenu(m, 'a', "Iguales", s1);
	int r2 = menu_agregar_submenu(m, 'b', "Iguales", s2);

	pa2m_afirmar(r1 == 0 && r2 == 0,
		     "Se pueden agregar submenus con la misma descripción");

	menu_destruir(m);
}
void test_menu_agregar_submenu_anidado()
{
	menu_t *m = menu_crear("Raiz");
	menu_t *a = menu_crear("A");
	menu_t *b = menu_crear("B");
	menu_t *c = menu_crear("C");

	int r1 = menu_agregar_submenu(m, 'a', "A", a);
	int r2 = menu_agregar_submenu(a, 'b', "B", b);
	int r3 = menu_agregar_submenu(b, 'c', "C", c);

	pa2m_afirmar(r1 == 0 && r2 == 0 && r3 == 0,
		     "Se pueden agregar submenus anidados correctamente ");

	menu_destruir(m);
}
void test_menu_agregar_submenu_en_menu_vacio()
{
	menu_t *menu = menu_crear("Menu principal");
	menu_t *sub = menu_crear("Submenu");

	int r = menu_agregar_submenu(menu, 'a', "Abrir submenu", sub);

	pa2m_afirmar(
		r == 0,
		"Se puede agregar un submenu correctamente en un menu vacío");

	menu_destruir(menu);
}
void test_menu_agregar_submenu_prueba_estress()
{
	menu_t *menu = menu_crear("Menu");

	// Array para guardar todos los submenus creados
	menu_t *subs[5000];

	bool todo_ok = true;

	for (int i = 0; i < 5000; i++) {
		char tecla = (char)(i % 26) + 'a';
		subs[i] = menu_crear("Sub");

		int r = menu_agregar_submenu(menu, tecla, "Stress", subs[i]);
		if (r != 0) {
			todo_ok = false;
			break;
		}
	}

	pa2m_afirmar(
		todo_ok,
		"Prueba de estrés: Agregar 5000 submenús no produce errores");

	menu_destruir(menu);
}

void tests_menu_agregar_submenu()
{
	test_menu_agregar_submenu_menu_null();
	test_menu_agregar_submenu_descripcion_null();
	test_menu_agregar_submenu_submenu_null();
	test_menu_agregar_submenu_correctamente();
	test_menu_agregar_submenu_correctamente_varios_submenus();
	test_menu_agregar_submenu_aumenta_cantidad();
	test_menu_agregar_submenu_descripcion_larga();
	test_menu_agregar_submenu_descripcion_corta();
	test_menu_agregar_submenu_descripcion_emojis();
	test_menu_agregar_submenu_descripcion_simbolos();
	test_menu_agregar_submenu_descripciones_iguales();
	test_menu_agregar_submenu_anidado();
	test_menu_agregar_submenu_en_menu_vacio();
	test_menu_agregar_submenu_prueba_estress();
}
//-------------------------------------------------
void test_menu_get_estilo_menu_null()
{
	int r = menu_get_estilo(NULL);
	pa2m_afirmar(r == -1,
		     "Obtener el estilo de un menu NULL devuelve error ");
}
void test_menu_get_estilo_recién_creado()
{
	menu_t *m = menu_crear("Menu");

	int estilo = menu_get_estilo(m);

	pa2m_afirmar(
		estilo == MENU_ESTILO_SIMPLE,
		"Se obtiene el estilo correctamente en un menu recién creado");

	menu_destruir(m);
}
void test_menu_get_estilo_prueba_estres()
{
	menu_t *m = menu_crear("Menu");

	menu_set_estilo(m, 1);

	bool ok = true;
	for (int i = 0; i < 5000; i++) {
		if (menu_get_estilo(m) != 1) {
			ok = false;
			break;
		}
	}

	pa2m_afirmar(
		ok,
		"Prueba estres: Se puede llamar a get_estilo +5000 veces sin error");

	menu_destruir(m);
}

void tests_menu_get_estilo()
{
	test_menu_get_estilo_menu_null();
	test_menu_get_estilo_recién_creado();
	test_menu_get_estilo_prueba_estres();
}
//-------------------------------------------------
void test_menu_set_estilo_menu_null()
{
	int r = menu_set_estilo(NULL, 0);
	pa2m_afirmar(r == -1,
		     "Setear el estilo de un menu NULL devuelve error NULL");
}
void test_menu_set_estilo_recién_creado()
{
	menu_t *m = menu_crear("Menu");

	int r = menu_set_estilo(m, MENU_ESTILO_COLORES);

	pa2m_afirmar(
		r == 0,
		"Se puede setear el estilo correctamente en un menu recién creado");

	menu_destruir(m);
}
void test_menu_set_estilo_correctamante()
{
	menu_t *m = menu_crear("Menu");

	menu_set_estilo(m, MENU_ESTILO_COLORES);
	int estilo = menu_get_estilo(m);
	pa2m_afirmar(estilo == 1,
		     "Cambiar el estilo del menu funciona correctamente");
	menu_destruir(m);
}
void test_menu_set_estilo_invalido()
{
	menu_t *m = menu_crear("Menu");

	int r = menu_set_estilo(m, 20); // fuera de rango

	pa2m_afirmar(r == -6, "Setear un estilo inválido devuelve error");

	menu_destruir(m);
}
void test_menu_set_estilo_prueba_estres()
{
	menu_t *m = menu_crear("Menu");

	menu_set_estilo(m, 1);

	bool ok = true;
	for (int i = 0; i < 5000; i++) {
		if (menu_set_estilo(m, 1) != 0) {
			ok = false;
			break;
		}
	}

	pa2m_afirmar(
		ok,
		"Prueba estres: Se puede setear el estilo +5000 veces sin error");

	menu_destruir(m);
}

void tests_menu_set_estilo()
{
	test_menu_set_estilo_menu_null();
	test_menu_set_estilo_recién_creado();
	test_menu_set_estilo_correctamante();
	test_menu_set_estilo_invalido();
	test_menu_set_estilo_prueba_estres();
}
//-------------------------------------------------
void test_menu_ejecutar_menu_null()
{
	void *r = menu_ejecutar(NULL, 'a', NULL);
	pa2m_afirmar(r == NULL,
		     "Ejecutar una tecla en un menú NULL devuelve NULL");
}
void test_menu_ejecutar_correctamente_varias_acciones()
{
	menu_t *m = menu_crear("Menu");

	menu_agregar_opcion(m, 'a', "Int", accion_int);
	menu_agregar_opcion(m, 'b', "Double", accion_double);
	menu_agregar_opcion(m, 'c', "Bool", accion_bool);

	int *ri = menu_ejecutar(m, 'a', NULL);
	double *rd = menu_ejecutar(m, 'b', NULL);
	bool *rb = menu_ejecutar(m, 'c', NULL);

	pa2m_afirmar(ri && rd && rb,
		     "Ejecutar correctamente varias acciones a la vez");

	menu_destruir(m);
}
void test_menu_ejecutar_correctamente_tipos_acciones()
{
	menu_t *m = menu_crear("Menu");

	menu_agregar_opcion(m, '1', "int", accion_int);
	menu_agregar_opcion(m, '2', "double", accion_double);
	menu_agregar_opcion(m, '3', "bool", accion_bool);
	menu_agregar_opcion(m, '4', "char", accion_char);
	menu_agregar_opcion(m, '5', "string", accion_string);
	menu_agregar_opcion(m, '6', "array", accion_array);
	menu_agregar_opcion(m, '7', "struct", accion_struct);
	menu_agregar_opcion(m, '8', "null", accion_null);

	int *ri = menu_ejecutar(m, '1', NULL);
	double *rd = menu_ejecutar(m, '2', NULL);
	bool *rb = menu_ejecutar(m, '3', NULL);
	char *rc = menu_ejecutar(m, '4', NULL);
	char *rs = menu_ejecutar(m, '5', NULL);
	int *arr = menu_ejecutar(m, '6', NULL);
	struct struct_prueba *st = menu_ejecutar(m, '7', NULL);
	void *rn = menu_ejecutar(m, '8', NULL);

	pa2m_afirmar(ri && *ri == 7,
		     "Se ejecutan correctamente acciones que devuelven int");
	pa2m_afirmar(rd && *rd == 3.14,
		     "Se ejecutan correctamente acciones que devuelven double");
	pa2m_afirmar(rb && *rb == true,
		     "Se ejecutan correctamente acciones que devuelven bool");
	pa2m_afirmar(rc && *rc == 'Z',
		     "Se ejecutan correctamente acciones que devuelven char");
	pa2m_afirmar(strcmp(rs, "Hola") == 0,
		     "Se ejecutan correctamente acciones que devuelven string");
	pa2m_afirmar(arr && arr[0] == 1 && arr[2] == 3,
		     "Se ejecutan correctamente acciones que devuelven arrays");
	pa2m_afirmar(
		st && st->x == 10 && st->y == 2.5,
		"Se ejecutan correctamente acciones que devuelven structs");
	pa2m_afirmar(rn == NULL,
		     "Se ejecutan correctamente acciones que devuelven null");

	menu_destruir(m);
}
void test_menu_ejecutar_correctamente_misma_accion()
{
	menu_t *m = menu_crear("Menu");

	menu_agregar_opcion(m, 'a', "uno", accion_int);
	menu_agregar_opcion(m, 'b', "dos", accion_int);
	menu_agregar_opcion(m, 'c', "tres", accion_int);

	int *r1 = menu_ejecutar(m, 'a', NULL);
	int *r2 = menu_ejecutar(m, 'b', NULL);
	int *r3 = menu_ejecutar(m, 'c', NULL);

	bool ok = (r1 && *r1 == 7) && (r2 && *r2 == 7) && (r3 && *r3 == 7);

	pa2m_afirmar(
		ok,
		"Tres teclas distintas ejecutan la misma accion correctamente");

	menu_destruir(m);
}
void test_menu_ejecutar_correctamente_acciones_con_contexto()
{
	menu_t *m = menu_crear("Menu");

	menu_agregar_opcion(m, 'x', "doble", accion_con_contexto);

	int valor = 21;
	int *r = menu_ejecutar(m, 'x', &valor);

	pa2m_afirmar(r && *r == 42,
		     "Ejecutar con contexto usa correctamente el contexto");

	menu_destruir(m);
}
void test_menu_ejecutar_prueba_estres()
{
	menu_t *m = menu_crear("Stress");

	for (int i = 0; i < 5000; i++) {
		char tecla = (char)((i % 26) + 'a');
		menu_agregar_opcion(m, tecla, "op", accion_int);
	}

	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		char tecla = (char)((i % 26) + 'a');
		int *r = menu_ejecutar(m, tecla, NULL);

		if (!r || *r != 7) {
			ok = false;
			break;
		}
	}

	pa2m_afirmar(
		ok,
		"Prueba estres: ejecutar 5000 acciones funciona correctamente");

	menu_destruir(m);
}

void tests_menu_ejecutar()
{
	test_menu_ejecutar_menu_null();
	test_menu_ejecutar_correctamente_varias_acciones();
	test_menu_ejecutar_correctamente_tipos_acciones();
	test_menu_ejecutar_correctamente_misma_accion();
	test_menu_ejecutar_correctamente_acciones_con_contexto();
	test_menu_ejecutar_prueba_estres();
}
//-------------------------------------------------
void test_menu_destruir_menu_null()
{
	menu_destruir(NULL);
	pa2m_afirmar(true, "Destruir un menú NULL no produce errores");
}
void test_menu_destruir_menu_vacio()
{
	menu_t *m = menu_crear("Menu vacío");

	menu_destruir(m);

	pa2m_afirmar(true, "Destruir menú vacío funciona correctamente");
}
void test_menu_destruir_menu_con_opciones()
{
	menu_t *m = menu_crear("Menu");

	menu_agregar_opcion(m, 'a', "Int", accion_int);
	menu_agregar_opcion(m, 'b', "Double", accion_double);

	menu_destruir(m);

	pa2m_afirmar(true, "Destruir menú con opciones no produce errores");
}
void test_menu_destruir_menu_con_submenus()
{
	menu_t *m = menu_crear("Principal");
	menu_t *sub = menu_crear("Submenu");

	menu_agregar_opcion(m, 's', "da", NULL);
	menu_agregar_submenu(m, 'x', "Adasd", sub);

	menu_destruir(sub);
	menu_destruir(m);

	pa2m_afirmar(
		true,
		"Destruir menú con submenús asociados funciona correctamente");
}

void tests_menu_destruir()
{
	test_menu_destruir_menu_null();
	test_menu_destruir_menu_vacio();
	test_menu_destruir_menu_con_opciones();
	test_menu_destruir_menu_con_submenus();
}

//---------------------------------------------------OPCION--------------------------------------------------------
void test_opcion_crear_descripcion_null()
{
	opcion_t *op = opcion_crear(NULL, 'a', NULL, NULL);
	pa2m_afirmar(op == NULL,
		     "Crear una opción con descripción NULL devuelve NULL");
}
void test_opcion_crear_descripcion_larga()
{
	opcion_t *op = opcion_crear(
		NULL, 'x',
		"Descripcion largaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
		NULL);

	pa2m_afirmar(op != NULL, "Se puede crear opción con descripción larga");

	opcion_destruir(op);
}
void test_opcion_crear_descripcion_corta()
{
	opcion_t *op = opcion_crear(NULL, 'z', "Hi", NULL);

	pa2m_afirmar(op != NULL, "Crear opción con descripción corta funciona");

	opcion_destruir(op);
}
void test_opcion_crear_descripcion_caracteres_especiales()
{
	opcion_t *op = opcion_crear(NULL, 'k', "Ñandú áéíóú !@#$", NULL);

	pa2m_afirmar(op != NULL,
		     "Crear opción con caracteres especiales funciona");

	opcion_destruir(op);
}
void test_opcion_crear_descripcion_emojis()
{
	opcion_t *op = opcion_crear(NULL, 'e', "🔥 Emoji test 💀✨", NULL);

	pa2m_afirmar(op != NULL, "Crear opción con emojis funciona");

	opcion_destruir(op);
}
void test_opcion_crear_con_accion()
{
	opcion_t *op = opcion_crear(NULL, 'a', "Acción", accion_int);

	pa2m_afirmar(op != NULL, "Crear opción con acción funciona");

	opcion_destruir(op);
}
void test_opcion_crear_con_submenu()
{
	menu_t *sub_menu = menu_crear("Submenu");

	opcion_t *op = opcion_crear(sub_menu, 's', "Submenú", NULL);

	pa2m_afirmar(op != NULL, "Crear opción con submenu funciona");

	opcion_destruir(op);
	menu_destruir(sub_menu);
}
void test_opcion_crear_descripcion_repetida()
{
	const char *desc = "Repetida";

	opcion_t *op1 = opcion_crear(NULL, '1', desc, NULL);
	opcion_t *op2 = opcion_crear(NULL, '2', desc, NULL);

	pa2m_afirmar(op1 && op2,
		     "Crear dos opciones con misma descripción funciona");
	pa2m_afirmar(op1 && op2,
		     "Se pueden crear múltiples opciones continuamente");

	opcion_destruir(op1);
	opcion_destruir(op2);
}
void test_opcion_crear_accion_repetida()
{
	opcion_t *op1 = opcion_crear(NULL, 'a', "A", accion_double);
	opcion_t *op2 = opcion_crear(NULL, 'b', "B", accion_double);

	pa2m_afirmar(op1 && op2,
		     "Crear dos opciones con la misma acción funciona");

	opcion_destruir(op1);
	opcion_destruir(op2);
}
void test_opcion_crear_con_submenu_varias_veces()
{
	menu_t *sub_menu = menu_crear("Submenu");
	menu_t *sub_menu2 = menu_crear("Submenu2");
	menu_t *sub_menu3 = menu_crear("Submenu3");

	opcion_t *op = opcion_crear(sub_menu, 's', "Submenú", NULL);
	opcion_t *op2 = opcion_crear(sub_menu2, 'b', "Submenú2", NULL);
	opcion_t *op3 = opcion_crear(sub_menu3, 'd', "Submenú3", NULL);

	pa2m_afirmar(
		op && op2 && op3,
		"Crear opción con submenu varias veces consecutivas funciona");

	opcion_destruir(op);
	opcion_destruir(op2);
	opcion_destruir(op3);
	menu_destruir(sub_menu);
}

void tests_opcion_crear()
{
	test_opcion_crear_descripcion_null();
	test_opcion_crear_descripcion_larga();
	test_opcion_crear_descripcion_corta();
	test_opcion_crear_descripcion_caracteres_especiales();
	test_opcion_crear_descripcion_emojis();
	test_opcion_crear_con_accion();
	test_opcion_crear_con_submenu();
	test_opcion_crear_descripcion_repetida();
	test_opcion_crear_accion_repetida();
	test_opcion_crear_con_submenu_varias_veces();
}
//-------------------------------------------------
void test_opcion_tecla_null()
{
	char t = opcion_tecla(NULL);
	pa2m_afirmar(t == 0, "Devuelve 0 cuando se le pasa un opcion NULL");
}
void test_opcion_tecla_correcta()
{
	opcion_t *op = opcion_crear(NULL, 'x', "Hola", accion_int);
	pa2m_afirmar(opcion_tecla(op) == 'x', "Devuelve la tecla correcta");

	opcion_destruir(op);
}
void test_opcion_tecla_caracteres_especiales()
{
	opcion_t *op = opcion_crear(NULL, '#', "Simbolo", accion_int);
	pa2m_afirmar(opcion_tecla(op) == '#',
		     "Funciona con caracteres especiales");

	opcion_destruir(op);
}
void test_opcion_tecla_prueba_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		char tecla = (char)((i % 26) + 'a');

		opcion_t *op = opcion_crear(NULL, tecla, "Desc", accion_null);
		if (!op) {
			ok = false;
			break;
		}

		char t = opcion_tecla(op);
		if (t != tecla) {
			ok = false;
			opcion_destruir(op);
			break;
		}

		opcion_destruir(op);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres:Más de 5000 lecturas de tecla pudieron realizarse sin fallar");
}

void tests_opcion_tecla()
{
	test_opcion_tecla_null();
	test_opcion_tecla_correcta();
	test_opcion_tecla_caracteres_especiales();
	test_opcion_tecla_prueba_estres();
}
//-------------------------------------------------
void test_opcion_descripcion_null()
{
	char *d = opcion_descripcion(NULL);
	pa2m_afirmar(d == NULL, "Devuelve NULL si se le pasa opcion NULL");
}
void test_opcion_descripcion_correcta()
{
	opcion_t *op = opcion_crear(NULL, 'a', "MiDesc", accion_int);

	pa2m_afirmar(strcmp(opcion_descripcion(op), "MiDesc") == 0,
		     "Devuelve la descripción correcta");

	opcion_destruir(op);
}
void test_opcion_descripcion_larga()
{
	const char *txt =
		"Descripcion extremadamente larga para probar que la "
		"copia de strings funciona correctamente incluso si "
		"la descripción es muy extensa y contiene muchos caracteres";

	opcion_t *op = opcion_crear(NULL, 'a', txt, accion_int);

	pa2m_afirmar(strcmp(opcion_descripcion(op), txt) == 0,
		     "Funciona con descripciones largas");

	opcion_destruir(op);
}
void test_opcion_descripcion_caracteres_especiales()
{
	opcion_t *op = opcion_crear(NULL, 'a', "@#$% &*!", accion_int);

	pa2m_afirmar(strcmp(opcion_descripcion(op), "@#$% &*!") == 0,
		     "Soporta caracteres especiales");

	opcion_destruir(op);
}
void test_opcion_descripcion_emojis()
{
	opcion_t *op = opcion_crear(NULL, 'a', "🔥✨Hola🍀", accion_int);

	pa2m_afirmar(strcmp(opcion_descripcion(op), "🔥✨Hola🍀") == 0,
		     "Soporta descripciones con emojis y unicode");

	opcion_destruir(op);
}
void test_opcion_descripcion_prueba_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "Desc-%d", i);

		opcion_t *op = opcion_crear(NULL, 'x', buffer, accion_null);
		if (!op) {
			ok = false;
			break;
		}

		char *desc = opcion_descripcion(op);
		if (!desc || strcmp(desc, buffer) != 0) {
			ok = false;
			opcion_destruir(op);
			break;
		}

		opcion_destruir(op);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres:Más de 5000 descripciones pudieron ser consultadas sin errores");
}

void tests_opcion_descripcion()
{
	test_opcion_descripcion_null();
	test_opcion_descripcion_correcta();
	test_opcion_descripcion_larga();
	test_opcion_descripcion_caracteres_especiales();
	test_opcion_descripcion_emojis();
	test_opcion_descripcion_prueba_estres();
}
//-------------------------------------------------
void test_opcion_accion_null()
{
	menu_action_t a = opcion_accion(NULL);
	pa2m_afirmar(a == NULL, "Devuelve NULL cuando se le pasa opcion null");
}
void test_opcion_accion_correcta()
{
	opcion_t *op = opcion_crear(NULL, 'a', "X", accion_double);

	pa2m_afirmar(opcion_accion(op) == accion_double,
		     "Devuelve correctamente el puntero a accion");

	opcion_destruir(op);
}
void test_opcion_accion_distintas()
{
	opcion_t *op1 = opcion_crear(NULL, '1', "Int", accion_int);
	opcion_t *op2 = opcion_crear(NULL, '2', "Bool", accion_bool);

	pa2m_afirmar(opcion_accion(op1) == accion_int &&
			     opcion_accion(op2) == accion_bool,
		     "Maneja correctamente acciones distintas");

	opcion_destruir(op1);
	opcion_destruir(op2);
}
void test_opcion_accion_repite_misma_accion()
{
	opcion_t *op1 = opcion_crear(NULL, 'a', "Desc1", accion_string);
	opcion_t *op2 = opcion_crear(NULL, 'b', "Desc2", accion_string);

	pa2m_afirmar(opcion_accion(op1) == opcion_accion(op2),
		     "Funciona correctamente con acciones repetidas");

	opcion_destruir(op1);
	opcion_destruir(op2);
}
void test_opcion_accion_prueba_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		// alterna entre varias acciones para que no siempre sea la misma
		menu_action_t acciones[] = { accion_null, accion_bool,
					     accion_int, accion_double,
					     accion_string };
		menu_action_t accion = acciones[i % 5];

		opcion_t *op = opcion_crear(NULL, 'y', "Test", accion);
		if (!op) {
			ok = false;
			break;
		}

		if (opcion_accion(op) != accion) {
			ok = false;
			opcion_destruir(op);
			break;
		}

		opcion_destruir(op);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres:Más de 5000 acciones pudieron verificarse sin fallas");
}

void tests_opcion_accion()
{
	test_opcion_accion_null();
	test_opcion_accion_correcta();
	test_opcion_accion_distintas();
	test_opcion_accion_repite_misma_accion();
	test_opcion_accion_prueba_estres();
}
//-------------------------------------------------
void test_opcion_destruir_null()
{
	opcion_destruir(NULL);

	pa2m_afirmar(true, "NULL pudo ser pasado sin provocar crash");
}
void test_opcion_destruir_opcion_simple()
{
	opcion_t *op = opcion_crear(NULL, 'a', "Hola", accion_int);

	opcion_destruir(op);

	pa2m_afirmar(true, "Una opción simple pudo liberarse sin crash");
}
void test_opcion_destruir_opcion_con_descripcion_larga()
{
	char *txt =
		"Una descripcion extremadamente larga para asegurar que "
		"la liberación de memoria pudo hacerse aun con strings grandes";

	opcion_t *op = opcion_crear(NULL, 'a', txt, accion_int);

	opcion_destruir(op);

	pa2m_afirmar(
		true,
		"Una opción con descripción larga pudo liberarse correctamente");
}
void test_opcion_destruir_opcion_con_emojis()
{
	opcion_t *op = opcion_crear(NULL, 'a', "🔥✨Hola🍀", accion_string);

	opcion_destruir(op);

	pa2m_afirmar(
		true,
		"Una opción con emojis se pudo eliminar sin provocar fallo");
}
void test_opcion_destruir_opcion_con_accion()
{
	opcion_t *op = opcion_crear(NULL, 'a', "X", accion_double);

	opcion_destruir(op);

	pa2m_afirmar(
		true,
		"Una opción que tenía una acción pudo ser liberada correctamente");
}
void test_opcion_destruir_descripcion_simbolos()
{
	const char *txt = "#@!!$%&*()_+[]{};:.,<>/?\\|`~";
	opcion_t *op = opcion_crear(NULL, '!', txt, accion_null);

	opcion_destruir(op);

	pa2m_afirmar(
		true,
		"Una opción con descripción de símbolos pudo ser liberada sin fallo");
}
void test_opcion_destruir_descripcion_corta()
{
	opcion_t *op = opcion_crear(NULL, 't', "X", accion_null);

	opcion_destruir(op);

	pa2m_afirmar(
		true,
		"Una opción con descripción de 1 caracter pudo ser liberada correctamente");
}
void test_opcion_destruir_prueba_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		char tecla = (char)((i % 26) + 'a');
		opcion_t *op = opcion_crear(NULL, tecla, "Desc", accion_null);

		if (!op) {
			ok = false;
			break;
		}

		opcion_destruir(op);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres: pudieron ser liberadas sin error +5000 opciones");
}

void tests_opcion_destruir()
{
	test_opcion_destruir_null();
	test_opcion_destruir_opcion_simple();
	test_opcion_destruir_opcion_con_descripcion_larga();
	test_opcion_destruir_opcion_con_emojis();
	test_opcion_destruir_opcion_con_accion();
	test_opcion_destruir_descripcion_simbolos();
	test_opcion_destruir_descripcion_corta();
	test_opcion_destruir_prueba_estres();
}

//----------------------------------------------------JUEGO-------------------------------------------------------
void test_juego_crear_tp1_null()
{
	pa2m_afirmar(juego_crear(NULL, "Ana", "Luis") == NULL,
		     "Devuelve NULL si tp1 es NULL");
}
void test_juego_crear_nombre1_null()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");

	pa2m_afirmar(juego_crear(tp1, NULL, "Luis") == NULL,
		     "Devuelve NULL si nombre 1 es NULL");
	tp1_destruir(tp1);
}
void test_juego_crear_nombre2_null()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");

	pa2m_afirmar(juego_crear(tp1, "Luca", NULL) == NULL,
		     "Devuelve NULL si nombre 2 es NULL");
	tp1_destruir(tp1);
}
void test_juego_crear_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");

	juego_t *juego = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego != NULL, "Crea correctamente un juego válido");

	juego_destruir(juego);
	tp1_destruir(tp1);
}
void test_juego_crear_dos_juegos_distintos()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");

	juego_t *j1 = juego_crear(tp1, "Ana", "Luis");
	juego_t *j2 = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(j1 != NULL && j2 != NULL && j1 != j2,
		     "Se pueden crear dos juegos distintos correctamente");

	juego_destruir(j1);
	juego_destruir(j2);
	tp1_destruir(tp1);
}

void tests_juego_crear()
{
	test_juego_crear_tp1_null();
	test_juego_crear_nombre1_null();
	test_juego_crear_nombre2_null();
	test_juego_crear_correctamente();
}
//-------------------------------------------------
void test_juego_jugador_1_juego_null()
{
	pa2m_afirmar(juego_jugador_1(NULL) == NULL,
		     "Devuelve NULL si el juego es NULL");
}
void test_juego_jugador_1_correcto()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_jugador_1(j) != NULL, "Devuelve un jugador válido");
	pa2m_afirmar(strcmp(jugador_obtener_nombre(juego_jugador_1(j)),
			    "Ana") == 0,
		     "Devuelve al jugador correcto");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_jugador_1_prueba_estres()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		if (juego_jugador_1(j) == NULL) {
			ok = false;
			break;
		}
		juego_siguiente_jugador(j); // Alternamos turnos
	}

	pa2m_afirmar(ok,
		     "Prueba estres:Mantiene jugador válido tras +5000 turnos");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_jugador_1()
{
	test_juego_jugador_1_juego_null();
	test_juego_jugador_1_correcto();
	test_juego_jugador_1_prueba_estres();
}
//-------------------------------------------------
void test_juego_jugador_2_juego_null()
{
	pa2m_afirmar(juego_jugador_2(NULL) == NULL,
		     "Devuelve NULL si el juego es NULL");
}
void test_juego_jugador_2_correcto()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_jugador_2(j) != NULL, "devuelve un jugador válido");
	pa2m_afirmar(strcmp(jugador_obtener_nombre(juego_jugador_2(j)),
			    "Luis") == 0,
		     "Devuelve al jugador correcto");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_jugador_2_prueba_estres()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		if (juego_jugador_2(j) == NULL) {
			ok = false;
			break;
		}
		juego_siguiente_jugador(j); // Alternamos turnos
	}

	pa2m_afirmar(ok,
		     "Prueba estres:Mantiene jugador válido tras +5000 turnos");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_jugador_2()
{
	test_juego_jugador_2_juego_null();
	test_juego_jugador_2_correcto();
	test_juego_jugador_2_prueba_estres();
}
//-------------------------------------------------
void test_juego_jugador_actual_juego_null()
{
	pa2m_afirmar(juego_jugador_actual(NULL) == NULL,
		     "Devuelve NULL si el juego es NULL");
}
void test_juego_jugador_actual_empieza_en_jugador_1()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_jugador_actual(j) == juego_jugador_1(j),
		     "Juego inicia correctamente como jugador 1");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_jugador_actual()
{
	test_juego_jugador_actual_juego_null();
	test_juego_jugador_actual_empieza_en_jugador_1();
}
//-------------------------------------------------
void test_juego_siguiente_jugador_juego_null()
{
	juego_siguiente_jugador(NULL);
	pa2m_afirmar(true, "No rompe si el juego es NULL");
}
void test_juego_siguiente_jugador_alternar_una_vez()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	jugador_t *primero = juego_jugador_actual(j);

	juego_siguiente_jugador(j);

	pa2m_afirmar(juego_jugador_actual(j) != primero,
		     "Cambia correctamente al jugador contrario");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_siguiente_jugador_alternar_muchas_veces()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	jugador_t *jugador_inicial = juego_jugador_actual(j);
	jugador_t *current = NULL;
	bool alterna_correlativamente = true;

	for (int i = 0; i < 20; i++) {
		juego_siguiente_jugador(j);
		current = juego_jugador_actual(j);
		if (i % 2 == 0 && current == jugador_inicial)
			alterna_correlativamente = false;
		if (i % 2 == 1 && current != jugador_inicial)
			alterna_correlativamente = false;
	}

	pa2m_afirmar(alterna_correlativamente,
		     "Alterna correctamente incluso tras muchas llamadas");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_siguiente_jugador()
{
	test_juego_siguiente_jugador_juego_null();
	test_juego_siguiente_jugador_alternar_una_vez();
	test_juego_siguiente_jugador_alternar_muchas_veces();
}
//-------------------------------------------------
void test_juego_carta_encontrada_juego_null()
{
	pa2m_afirmar(!juego_carta_encontrada(NULL, 0, 1),
		     "Devuelve false cuando el juego es NULL");
}
void test_juego_carta_encontrada_indices_invalidos()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(!juego_carta_encontrada(j, 9999, 10000),
		     "Devuelve false cuando indices no existen");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_carta_encontrada_correcta()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	size_t idx1 = 1;

	pa2m_afirmar(juego_carta_encontrada(j, idx1, idx1),
		     "Devuelve true cuando las cartas coinciden");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_carta_encontrada()
{
	test_juego_carta_encontrada_juego_null();
	test_juego_carta_encontrada_indices_invalidos();
	test_juego_carta_encontrada_correcta();
}
//-------------------------------------------------
void test_juego_registrar_jugada_juego_null()
{
	pa2m_afirmar(!juego_registrar_jugada(NULL, 0, 1, true),
		     "Devuelve false si el juego es NULL");
}
void test_juego_registrar_jugada_correcta_no_encontrada()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_registrar_jugada(j, 1, 2, false),
		     "Registra correctamente una jugada no encontrada");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_registrar_jugada_correcta_encontrada()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_registrar_jugada(j, 1, 8, true),
		     "Registra correctamente una jugada encontrada");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_registrar_jugada_elimina_carta_encontrada()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	size_t cantidad_inicial = juego_cartas_restantes(j);

	// Forzar que las cartas sean iguales
	struct pokemon *p1 = lista_buscar_elemento(juego_cartas_lista(j), 0);
	struct pokemon *p2 = lista_buscar_elemento(juego_cartas_lista(j), 1);
	if (p1 && p2)
		p2->id = p1->id;

	pa2m_afirmar(juego_registrar_jugada(j, 0, 1, true),
		     "Registrar jugada encontrada devuelve true");

	size_t cantidad_final = juego_cartas_restantes(j);

	pa2m_afirmar(
		cantidad_final == cantidad_inicial - 2,
		"Registrar jugada encontrada elimina correctamente las cartas de la lista");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_registrar_jugada()
{
	test_juego_registrar_jugada_juego_null();
	test_juego_registrar_jugada_correcta_no_encontrada();
	test_juego_registrar_jugada_correcta_encontrada();
	test_juego_registrar_jugada_elimina_carta_encontrada();
}
//-------------------------------------------------
void test_juego_terminado_juego_null()
{
	pa2m_afirmar(!juego_terminado(NULL),
		     "Devuelve false si el juego es NULL");
}
void test_juego_terminado_inicialmente()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(!juego_terminado(j),
		     "Devuelve false cuando el juego tiene cartas");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_juego_terminado_cuando_se_acaban_cartas()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	// Eliminar todas las cartas
	while (lista_cantidad(juego_cartas_lista(j)) > 0)
		lista_eliminar_elemento(juego_cartas_lista(j), 0);

	pa2m_afirmar(juego_terminado(j),
		     "Devuelve true cuando no quedan cartas");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_terminado()
{
	test_juego_terminado_juego_null();
	test_juego_terminado_inicialmente();
	test_juego_terminado_inicialmente();
}
//-------------------------------------------------
void test_juego_cartas_restantes_juego_null()
{
	pa2m_afirmar(juego_cartas_restantes(NULL) == 0,
		     "Devuelve 0 si el juego es NULL");
}
void test_juego_cartas_restantes_inicialmente()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	size_t cantidad = juego_cartas_restantes(j);
	pa2m_afirmar(cantidad == 18, "Devuelve 18 cartas inicialmente");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_cartas_restantes()
{
	test_juego_cartas_restantes_juego_null();
	test_juego_cartas_restantes_inicialmente();
}
//-------------------------------------------------
void test_juego_cartas_lista_juego_null()
{
	pa2m_afirmar(juego_cartas_lista(NULL) == NULL,
		     "Devuelve NULL si el juego es NULL");
}
void test_juego_cartas_lista_correcto()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_cartas_lista(j) != NULL,
		     "Devuelve lista de cartas válida");

	size_t cantidad = lista_cantidad(juego_cartas_lista(j));
	pa2m_afirmar((int)cantidad == 18,
		     "Lista de cartas inicializa con 18 cartas");
	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_cartas_lista()
{
	test_juego_cartas_lista_juego_null();
	test_juego_cartas_lista_correcto();
}
//-------------------------------------------------
void test_juego_ultimas_jugadas_juego_null()
{
	pa2m_afirmar(juego_ultimas_jugadas(NULL) == NULL,
		     "devuelve NULL si el juego es NULL");
}
void test_juego_ultimas_jugadas_correcto()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	pa2m_afirmar(juego_ultimas_jugadas(j) != NULL,
		     "Devuelve lista de últimas jugadas válida");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_registrar_ultimas_5_jugadas_menos_de_5()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	// Registrar 3 jugadas
	for (size_t i = 0; i < 3; i++)
		juego_registrar_jugada(j, i, i + 1, false);

	size_t cantidad = lista_cantidad(juego_ultimas_jugadas(j));
	pa2m_afirmar(cantidad == 3,
		     "Cantidad de ultimas jugadas es correcta (<5)");

	juego_destruir(j);
	tp1_destruir(tp1);
}
void test_registrar_ultimas_5_jugadas_maximo_5()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	for (size_t i = 0; i < 7; i++)
		juego_registrar_jugada(j, i, i + 1, false);

	size_t cantidad = lista_cantidad(juego_ultimas_jugadas(j));
	pa2m_afirmar(
		cantidad == 5,
		"Solo se mantienen 5 ultimas jugadas al superar el maximo");

	// Comprobar que la primera jugada (indice 0) fue eliminada
	jugada_registrada_t *primera =
		lista_buscar_elemento(juego_ultimas_jugadas(j), 0);
	pa2m_afirmar(
		(int)jugada_carta_1(primera) == 6,
		"La jugada mas reciente esta al principio tras eliminar antiguas");

	juego_destruir(j);
	tp1_destruir(tp1);
}

void tests_juego_ultimas_jugadas()
{
	test_juego_ultimas_jugadas_juego_null();
	test_juego_ultimas_jugadas_correcto();
	test_registrar_ultimas_5_jugadas_menos_de_5();
	test_registrar_ultimas_5_jugadas_maximo_5();
}
//-------------------------------------------------
void test_juego_destruir_juego_null()
{
	juego_destruir(NULL);
	pa2m_afirmar(true, "No rompe si el juego es NULL");
}
void test_juego_destruir_correctamente_sin_jugadas()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");

	juego_destruir(j);
	pa2m_afirmar(true,
		     "Juego destruido correctamente sin errores y sin jugadas");

	tp1_destruir(tp1);
}
void test_juego_destruir_correctamente_con_jugadas()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");
	juego_registrar_jugada(j, 0, 1, false);

	juego_destruir(j);
	pa2m_afirmar(true,
		     "Juego destruido correctamente sin errores con jugadas");

	tp1_destruir(tp1);
}
void test_juego_destruir_correctamente_sin_cartas()
{
	tp1_t *tp1 = tp1_leer_archivo("assets/pokemones.csv");
	juego_t *j = juego_crear(tp1, "Ana", "Luis");
	size_t cant;

	for (size_t i = 0; i < 10; i++)
		juego_registrar_jugada(j, 0, 1, true); //Fuerzo el encuentro

	cant = juego_cartas_restantes(j);

	juego_destruir(j);
	pa2m_afirmar(cant == 0,
		     "Juego destruido correctamente sin errores sin cartas");

	tp1_destruir(tp1);
}

void tests_juego_destruir()
{
	test_juego_destruir_juego_null();
	test_juego_destruir_correctamente_sin_jugadas();
	test_juego_destruir_correctamente_con_jugadas();
	test_juego_destruir_correctamente_sin_cartas();
}

//---------------------------------------------------JUGADOR--------------------------------------------------------
void test_jugador_crear_nombre_null()
{
	jugador_t *j = jugador_crear(NULL);

	pa2m_afirmar(
		j == NULL,
		"Devuelve NULL cuando se intenta crear un jugador con nombre NULL");
}
void test_jugador_crear_correctamente()
{
	jugador_t *j = jugador_crear("Luca");

	pa2m_afirmar(j != NULL,
		     "Crea correctamente un jugador con un nombre válido");

	jugador_destruir(j);
}
void test_jugador_crear_nombre_corto()
{
	jugador_t *j = jugador_crear("A");

	pa2m_afirmar(j != NULL,
		     "Crea correctamente un jugador con nombre de 1 caracter");

	jugador_destruir(j);
}
void test_jugador_crear_nombre_largo()
{
	jugador_t *j = jugador_crear(
		"Nombre extremadamente largo para verificar correcto manejo de memoria");

	pa2m_afirmar(j != NULL,
		     "Crea correctamente un jugador con un nombre muy largo");

	jugador_destruir(j);
}
void test_jugador_crear_nombre_caracteres_especiales()
{
	jugador_t *j = jugador_crear("Luca✨🔥_#@!");

	pa2m_afirmar(
		j != NULL,
		"Crea correctamente un jugador con caracteres especiales y unicode");

	jugador_destruir(j);
}
void test_jugador_crear_nombres_iguales()
{
	jugador_t *j1 = jugador_crear("Luca");
	jugador_t *j2 = jugador_crear("Luca");

	pa2m_afirmar(
		j1 != NULL && j2 != NULL,
		"Crea correctamente dos jugadores distintos con el mismo nombre");

	jugador_destruir(j1);
	jugador_destruir(j2);
}
void test_jugador_crear_prueba_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		char nombre[32];
		snprintf(nombre, sizeof(nombre), "Jugador_%d", i);

		jugador_t *j = jugador_crear(nombre);
		if (!j) {
			ok = false;
			break;
		}

		jugador_destruir(j);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres:Crea y destruye +5000 jugadores correctamente sin pérdidas de memoria");
}

void tests_jugador_crear()
{
	test_jugador_crear_nombre_null();
	test_jugador_crear_correctamente();
	test_jugador_crear_nombre_corto();
	test_jugador_crear_nombre_largo();
	test_jugador_crear_nombre_caracteres_especiales();
	test_jugador_crear_nombres_iguales();
	test_jugador_crear_prueba_estres();
}
//-------------------------------------------------
void test_jugador_obtener_nombre_jugador_null()
{
	char *nombre = jugador_obtener_nombre(NULL);

	pa2m_afirmar(nombre == NULL, "Devuelve NULL cuando el jugador es NULL");
}
void test_jugador_obtener_nombre_correctamente()
{
	jugador_t *j = jugador_crear("Luca");

	pa2m_afirmar(strcmp(jugador_obtener_nombre(j), "Luca") == 0,
		     "Obtiene correctamente el nombre del jugador");

	jugador_destruir(j);
}
void test_jugador_obtener_nombre_corto()
{
	jugador_t *j = jugador_crear("A");

	pa2m_afirmar(strcmp(jugador_obtener_nombre(j), "A") == 0,
		     "Obtiene correctamente un nombre de un solo caracter");

	jugador_destruir(j);
}
void test_jugador_obtener_nombre_largo()
{
	jugador_t *j =
		jugador_crear("NombreExtremadamenteLargoDeJugador123456789");

	pa2m_afirmar(strcmp(jugador_obtener_nombre(j),
			    "NombreExtremadamenteLargoDeJugador123456789") == 0,
		     "Obtiene correctamente un nombre largo");

	jugador_destruir(j);
}
void test_jugador_obtener_nombre_caracteres_especiales()
{
	jugador_t *j = jugador_crear("🔥✨Luca_Pro!#%&");

	pa2m_afirmar(
		strcmp(jugador_obtener_nombre(j), "🔥✨Luca_Pro!#%&") == 0,
		"Obtiene correctamente un nombre con caracteres especiales y unicode");

	jugador_destruir(j);
}

void tests_jugador_obtener_nombre()
{
	test_jugador_obtener_nombre_jugador_null();
	test_jugador_obtener_nombre_correctamente();
	test_jugador_obtener_nombre_corto();
	test_jugador_obtener_nombre_largo();
	test_jugador_obtener_nombre_caracteres_especiales();
}
//-------------------------------------------------
void test_jugador_obtener_puntaje_jugador_null()
{
	pa2m_afirmar(jugador_obtener_puntaje(NULL) == 0,
		     "Devuelve 0 cuando el jugador es NULL");
}
void test_jugador_obtener_puntaje_inicial()
{
	jugador_t *j = jugador_crear("Luca");

	pa2m_afirmar(jugador_obtener_puntaje(j) == 0,
		     "Obtiene correctamente el puntaje inicial del jugador");

	jugador_destruir(j);
}
void test_jugador_obtener_puntaje_luego_de_sumar()
{
	jugador_t *j = jugador_crear("Luca");

	jugador_sumar_puntaje(j);
	jugador_sumar_puntaje(j);

	pa2m_afirmar(jugador_obtener_puntaje(j) == 2,
		     "Obtiene correctamente el puntaje luego de sumar puntos");

	jugador_destruir(j);
}
void test_jugador_obtener_puntaje_prueba_estres()
{
	jugador_t *j = jugador_crear("Jugador");

	for (int i = 0; i < 5000; i++)
		jugador_sumar_puntaje(j);

	pa2m_afirmar(
		jugador_obtener_puntaje(j) == 5000,
		"Prueba estres:Obtiene correctamente el puntaje después de miles de sumas consecutivas");

	jugador_destruir(j);
}

void tests_jugador_obtener_puntaje()
{
	test_jugador_obtener_puntaje_jugador_null();
	test_jugador_obtener_puntaje_inicial();
	test_jugador_obtener_puntaje_luego_de_sumar();
	test_jugador_obtener_puntaje_prueba_estres();
}
//-------------------------------------------------
void test_jugador_sumar_puntaje_jugador_null()
{
	jugador_sumar_puntaje(NULL);

	pa2m_afirmar(
		true,
		"Ignora correctamente la operación cuando el jugador es NULL");
}
void test_jugador_sumar_puntaje_una_vez()
{
	jugador_t *j = jugador_crear("Luca");

	jugador_sumar_puntaje(j);

	pa2m_afirmar(jugador_obtener_puntaje(j) == 1,
		     "Suma correctamente un punto al jugador");

	jugador_destruir(j);
}
void test_jugador_sumar_puntaje_varias_veces()
{
	jugador_t *j = jugador_crear("Luca");

	jugador_sumar_puntaje(j);
	jugador_sumar_puntaje(j);
	jugador_sumar_puntaje(j);

	pa2m_afirmar(jugador_obtener_puntaje(j) == 3,
		     "Acumula correctamente varios puntos consecutivos");

	jugador_destruir(j);
}
void test_jugador_sumar_puntaje_prueba_estres()
{
	pa2m_nuevo_grupo("Test jugador_sumar_puntaje — prueba de estrés");

	jugador_t *j = jugador_crear("Jugador");

	for (int i = 0; i < 5000; i++)
		jugador_sumar_puntaje(j);

	pa2m_afirmar(
		jugador_obtener_puntaje(j) == 5000,
		"Prueba estres:Acumula correctamente +5000  incrementos consecutivos");

	jugador_destruir(j);
}

void tests_jugador_sumar_puntaje()
{
	test_jugador_sumar_puntaje_jugador_null();
	test_jugador_sumar_puntaje_una_vez();
	test_jugador_sumar_puntaje_varias_veces();
	test_jugador_sumar_puntaje_prueba_estres();
}
//-------------------------------------------------
void test_jugador_registrar_jugada_jugador_null()
{
	jugada_registrada_t *j = jugada_crear(1, 2, true);

	bool r = jugador_registrar_jugada(NULL, j);

	pa2m_afirmar(!r, "Devuelve false cuando el jugador es NULL");

	jugada_destruir(j);
}
void test_jugador_registrar_jugada_jugada_null()
{
	jugador_t *p = jugador_crear("Luca");

	bool r = jugador_registrar_jugada(p, NULL);

	pa2m_afirmar(!r, "Devuelve false cuando la jugada es NULL");

	jugador_destruir(p);
}
void test_jugador_registrar_jugada_correctamente()
{
	jugador_t *p = jugador_crear("Luca");
	jugada_registrada_t *j = jugada_crear(3, 7, true);

	bool r = jugador_registrar_jugada(p, j);

	pa2m_afirmar(r, "Registra correctamente una jugada en el jugador");

	jugador_destruir(p);
}
void test_jugador_registrar_jugada_varias_veces()
{
	jugador_t *p = jugador_crear("Luca");
	bool ok = true;

	for (size_t i = 0; i < 5; i++) {
		jugada_registrada_t *j = jugada_crear(i, (i + 1), true);
		if (!jugador_registrar_jugada(p, j))
			ok = false;
	}

	pa2m_afirmar(ok,
		     "Registra correctamente múltiples jugadas consecutivas");

	jugador_destruir(p);
}
void test_jugador_registrar_jugada_prueba_estres()
{
	jugador_t *p = jugador_crear("Jugador");
	bool ok = true;

	for (size_t i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(i, i + 2, (i % 2 == 0));
		if (!jugador_registrar_jugada(p, j)) {
			ok = false;
			break;
		}
	}

	pa2m_afirmar(ok, "Registra correctamente +5000  jugadas sin fallar");

	jugador_destruir(p);
}

void tests_jugador_registrar_jugada()
{
	test_jugador_registrar_jugada_jugador_null();
	test_jugador_registrar_jugada_jugada_null();
	test_jugador_registrar_jugada_correctamente();
	test_jugador_registrar_jugada_varias_veces();
	test_jugador_registrar_jugada_prueba_estres();
}
//-------------------------------------------------
void test_jugador_registro_jugadas_jugador_null()
{
	void *r = jugador_registro_jugadas(NULL);
	pa2m_afirmar(r == NULL, "Devuelve NULL si el jugador es NULL");
}
void test_jugador_registro_jugadas_lista_vacia()
{
	jugador_t *p = jugador_crear("Luca");
	void *r = jugador_registro_jugadas(p);
	pa2m_afirmar(r != NULL, "Devuelve la lista aunque esté vacía");
	pa2m_afirmar(lista_cantidad(r) == 0,
		     "La lista está vacía inicialmente");
	jugador_destruir(p);
}
void test_jugador_registro_jugadas_con_jugadas()
{
	jugador_t *p = jugador_crear("Luca");
	jugada_registrada_t *j1 = jugada_crear(1, 2, true);
	jugada_registrada_t *j2 = jugada_crear(3, 4, false);

	jugador_registrar_jugada(p, j1);
	jugador_registrar_jugada(p, j2);

	lista_t *r = jugador_registro_jugadas(p);
	pa2m_afirmar(r != NULL, "Devuelve la lista de jugadas");
	pa2m_afirmar(lista_cantidad(r) == 2, "La lista tiene 2 jugadas");

	jugador_destruir(p);
}

void tests_jugador_registro_jugadas()
{
	test_jugador_registro_jugadas_jugador_null();
	test_jugador_registro_jugadas_lista_vacia();
	test_jugador_registro_jugadas_con_jugadas();
}
//-------------------------------------------------
void test_jugador_destruir_jugador_null()
{
	jugador_destruir(NULL); // no debería hacer nada
	pa2m_afirmar(true, "No falla al destruir un jugador NULL");
}
void test_jugador_destruir_sin_jugadas()
{
	jugador_t *p = jugador_crear("Luca");
	jugador_destruir(p);
	pa2m_afirmar(true, "Destruye correctamente un jugador sin jugadas");
}
void test_jugador_destruir_varias_jugadas()
{
	jugador_t *p = jugador_crear("Luca");
	for (size_t i = 0; i < 5; i++) {
		jugada_registrada_t *j = jugada_crear(i, i + 1, true);
		jugador_registrar_jugada(p, j);
	}
	jugador_destruir(p);
	pa2m_afirmar(true,
		     "Destruye correctamente un jugador con varias jugadas");
}
void test_jugador_destruir_prueba_estres()
{
	jugador_t *p = jugador_crear("Jugador");
	for (size_t i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(i, i + 2, (i % 2 == 0));
		jugador_registrar_jugada(p, j);
	}
	jugador_destruir(p);
	pa2m_afirmar(
		true,
		"Prueba estres:Destruye correctamente un jugador con +5000 jugadas");
}

void tests_jugador_destruir()
{
	test_jugador_destruir_jugador_null();
	test_jugador_destruir_sin_jugadas();
	test_jugador_destruir_varias_jugadas();
	test_jugador_destruir_prueba_estres();
}
//---------------------------------------------------JUGADA--------------------------------------------------------
void test_jugada_crear_correctamente()
{
	jugada_registrada_t *j = jugada_crear(3, 7, false);

	pa2m_afirmar(j != NULL, "Se crea una jugada correctamente");
	pa2m_afirmar(
		j != NULL,
		"Crear jugada con unas cartas no encontradas funciona correctamente");

	jugada_destruir(j);
}
void test_jugada_crear_carta_encontrada()
{
	jugada_registrada_t *j = jugada_crear(5, 9, true);

	pa2m_afirmar(
		j != NULL,
		"Crear jugada con unas cartas encontradas funciona correctamente");

	jugada_destruir(j);
}
void test_jugada_crear_misma_jugada()
{
	jugada_registrada_t *j = jugada_crear(4, 5, false);
	jugada_registrada_t *j2 = jugada_crear(4, 5, false);

	pa2m_afirmar(
		j != NULL && j2 != NULL,
		"Crear varias jugada con cartas iguales funciona correctamente");

	free(j);
	free(j2);
}
void test_jugada_crear_test_estres()
{
	bool ok = true;

	for (int i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(
			(size_t)(i % 20), (size_t)(i + 3) % 20, false);
		if (!j) {
			ok = false;
			break;
		}
		free(j);
	}

	pa2m_afirmar(ok, "Prueba estres: se pueden crear +5000  jugadas");
}

void tests_jugada_crear()
{
	test_jugada_crear_correctamente();
	test_jugada_crear_carta_encontrada();
	test_jugada_crear_misma_jugada();
	test_jugada_crear_test_estres();
}
//-------------------------------------------------
void test_jugada_carta_1_jugada_null()
{
	pa2m_afirmar(jugada_carta_1(NULL) == 0,
		     "Devuelve 0 cuando la jugada es NULL");
}
void test_jugada_carta_1_correctamente()
{
	jugada_registrada_t *j = jugada_crear(12, 99, false);

	pa2m_afirmar((int)jugada_carta_1(j) == 12,
		     "Devuelve correctamente la primera carta");

	jugada_destruir(j);
}
void test_jugada_carta_1_prueba_estres()
{
	bool ok = true;

	for (size_t i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(i, i + 10, false);
		if (!j) {
			ok = false;
			break;
		}

		if (jugada_carta_1(j) != i) {
			ok = false;
			jugada_destruir(j);
			break;
		}

		jugada_destruir(j);
	}

	pa2m_afirmar(
		ok,
		"Devuelve correctamente la primera carta en una prueba de estrés");
}

void tests_jugada_carta_1()
{
	test_jugada_carta_1_jugada_null();
	test_jugada_carta_1_correctamente();
	test_jugada_carta_1_prueba_estres();
}
//-------------------------------------------------
void test_jugada_carta_2_jugada_null()
{
	pa2m_afirmar(jugada_carta_2(NULL) == 0,
		     "Devuelve 0 cuando la jugada es NULL");
}
void test_jugada_carta_2_correctamente()
{
	jugada_registrada_t *j = jugada_crear(44, 17, false);

	pa2m_afirmar((int)jugada_carta_2(j) == 17,
		     "Devuelve correctamente la segunda carta");

	jugada_destruir(j);
}
void test_jugada_carta_2_prueba_estres()
{
	bool ok = true;

	for (size_t i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(i + 3, i, false);
		if (!j) {
			ok = false;
			break;
		}

		if (jugada_carta_2(j) != i) {
			ok = false;
			jugada_destruir(j);
			break;
		}

		jugada_destruir(j);
	}

	pa2m_afirmar(
		ok,
		"Devuelve correctamente la segunda carta en una prueba de estrés");
}

void tests_jugada_carta_2()
{
	test_jugada_carta_2_jugada_null();
	test_jugada_carta_2_correctamente();
	test_jugada_carta_2_prueba_estres();
}
//-------------------------------------------------
void test_jugada_encontrado_jugada_null()
{
	pa2m_afirmar(jugada_encontrado(NULL) == false,
		     "Devuelve false cuando la jugada es NULL");
}
void test_jugada_encontrado_creada_true()
{
	jugada_registrada_t *j = jugada_crear(3, 7, true);

	pa2m_afirmar(
		jugada_encontrado(j) == true,
		"Indica correctamente que la jugada ha sido marcada como encontrada");

	jugada_destruir(j);
}
void test_jugada_encontrado_creada_false()
{
	jugada_registrada_t *j = jugada_crear(3, 7, false);

	pa2m_afirmar(
		jugada_encontrado(j) == false,
		"Indica correctamente que la jugada no ha sido marcada como encontrada");

	jugada_destruir(j);
}
void test_jugada_encontrado_prueba_estres()
{
	bool ok = true;

	for (size_t i = 0; i < 5000; i++) {
		bool encontrado = (i % 2 == 0);
		jugada_registrada_t *j = jugada_crear(i, i + 1, encontrado);

		if (!j) {
			ok = false;
			break;
		}

		if (jugada_encontrado(j) != encontrado) {
			ok = false;
			jugada_destruir(j);
			break;
		}

		jugada_destruir(j);
	}

	pa2m_afirmar(
		ok,
		"Prueba estres: Indica correctamente el estado encontrado en +5000 jugadas");
}

void tests_jugada_encontrado()
{
	test_jugada_encontrado_jugada_null();
	test_jugada_encontrado_creada_true();
	test_jugada_encontrado_creada_false();
	test_jugada_encontrado_prueba_estres();
}
//-------------------------------------------------
void test_jugada_destruir_jugada_null()
{
	jugada_destruir(NULL);

	pa2m_afirmar(true,
		     "No genera errores cuando se le pasa NULL al destruir");
}
void test_jugada_destruir_jugada_valida()
{
	jugada_registrada_t *j = jugada_crear(3, 7, true);

	jugada_destruir(j);

	pa2m_afirmar(true, "Destruye correctamente una jugada válida");
}
void test_jugada_destruir_prueba_estres()
{
	bool ok = true;

	for (size_t i = 0; i < 5000; i++) {
		jugada_registrada_t *j = jugada_crear(i, i + 1, (i % 2 == 0));
		if (!j) {
			ok = false;
			break;
		}
		jugada_destruir(j);
	}

	pa2m_afirmar(
		ok,
		"Pruena estres: Destruye correctamente +5000 jugadas sin fallar");
}

void tests_jugada_destruir()
{
	test_jugada_destruir_jugada_null();
	test_jugada_destruir_jugada_valida();
	test_jugada_destruir_prueba_estres();
}
//-----------------------------------------------------------------------------------------------------------

int main()
{
	pa2m_nuevo_grupo("============== TESTS MENU ===============");
	pa2m_nuevo_grupo("----- menu_crear -----");
	tests_menu_crear();

	pa2m_nuevo_grupo("----- menu_cantidad -----");
	tests_menu_cantidad();

	pa2m_nuevo_grupo("----- menu_agregar_opcion -----");
	tests_menu_agregar_opcion();

	pa2m_nuevo_grupo("----- menu_agregar_submenu -----");
	tests_menu_agregar_submenu();

	pa2m_nuevo_grupo("----- menu_set_estilo -----");
	tests_menu_set_estilo();

	pa2m_nuevo_grupo("----- menu_get_estilo -----");
	tests_menu_get_estilo();

	//Iterador interno para el menu mostrar
	pa2m_nuevo_grupo("----- menu_ejecutar -----");
	tests_menu_ejecutar();

	pa2m_nuevo_grupo("----- menu_destruir -----");
	tests_menu_destruir();

	pa2m_nuevo_grupo("============== TESTS OPCION ===============");
	pa2m_nuevo_grupo("----- opcion_crear -----");
	tests_opcion_crear();

	pa2m_nuevo_grupo("----- opcion_tecla -----");
	tests_opcion_tecla();

	pa2m_nuevo_grupo("----- opcion_descripcion -----");
	tests_opcion_descripcion();

	pa2m_nuevo_grupo("----- opcion_accion -----");
	tests_opcion_accion();

	pa2m_nuevo_grupo("----- opcion_destruir -----");
	tests_opcion_destruir();

	pa2m_nuevo_grupo("============== TESTS JUEGO ===============");
	pa2m_nuevo_grupo("----- juego_crear -----");
	tests_juego_crear();

	pa2m_nuevo_grupo("----- juego_jugador_1 -----");
	tests_juego_jugador_1();

	pa2m_nuevo_grupo("----- juego_jugador_2 -----");
	tests_juego_jugador_2();

	pa2m_nuevo_grupo("----- juego_jugador_actual -----");
	tests_juego_jugador_actual();

	pa2m_nuevo_grupo("----- juego_siguiente_jugador -----");
	tests_juego_siguiente_jugador();

	pa2m_nuevo_grupo("----- juego_carta_encontrada -----");
	tests_juego_carta_encontrada();

	pa2m_nuevo_grupo("----- juego_registrar_jugada -----");
	tests_juego_registrar_jugada();

	pa2m_nuevo_grupo("----- juego_terminado -----");
	tests_juego_terminado();

	pa2m_nuevo_grupo("----- juego_cartas_restantes -----");
	tests_juego_cartas_restantes();

	pa2m_nuevo_grupo("----- juego_cartas_lista -----");
	tests_juego_cartas_lista();

	pa2m_nuevo_grupo("----- juego_ultimas_jugadas -----");
	tests_juego_ultimas_jugadas();

	pa2m_nuevo_grupo("----- juego_destruir -----");
	tests_juego_destruir();

	pa2m_nuevo_grupo("============== TESTS JUGADOR ===============");
	pa2m_nuevo_grupo("----- jugador_crear -----");
	tests_jugador_crear();

	pa2m_nuevo_grupo("----- jugador_obtener_nombre -----");
	tests_jugador_obtener_nombre();

	pa2m_nuevo_grupo("----- jugador_obtener_puntaje -----");
	tests_jugador_obtener_puntaje();

	pa2m_nuevo_grupo("----- jugador_sumar_puntaje -----");
	tests_jugador_sumar_puntaje();

	pa2m_nuevo_grupo("----- jugador_registrar_jugada -----");
	tests_jugador_registrar_jugada();

	pa2m_nuevo_grupo("----- jugador_registro_jugadas -----");
	tests_jugador_registro_jugadas();

	pa2m_nuevo_grupo("----- jugador_destruir -----");
	tests_jugador_destruir();

	pa2m_nuevo_grupo("============== TESTS JUGADA ===============");
	pa2m_nuevo_grupo("----- jugada_crear -----");
	tests_jugada_crear();

	pa2m_nuevo_grupo("----- jugada_carta_1 -----");
	tests_jugada_carta_1();

	pa2m_nuevo_grupo("----- jugada_carta_2 -----");
	tests_jugada_carta_2();

	pa2m_nuevo_grupo("----- jugada_encontrado -----");
	tests_jugada_encontrado();

	pa2m_nuevo_grupo("----- jugada_destruir -----");
	tests_jugada_destruir();

	return pa2m_mostrar_reporte();
}
