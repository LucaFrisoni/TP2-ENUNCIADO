#ifndef ACTIONS_H
#define ACTIONS_H

#include "estructuras_de_datos/menu.h"

void *cargar_archivo(void *contexto);
void *buscar_por_id(void *contexto);
void *buscar_por_nombre(void *contexto);
void *mostrar_por_id(void *contexto);
void *mostrar_por_nombre(void *contexto);
void *menu_anterior(void *contexto);
void *jugar(void *contexto);
void *jugar_con_semilla(void *contexto);
void *cambiar_estilo(void *contexto);
void *salir(void *contexto);

#endif