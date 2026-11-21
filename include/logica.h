#include <stdbool.h>
#ifndef LOGICA_H
#define LOGICA_H

#include "entradas.h"

#define FILAS_JUEGO      5    //Filas y columnas donde se jugará
#define COLUMNAS_JUEGO   11
#define TUMBAS           8

#define PASTO            0
#define GIRASOL          1    //Plantas
#define LANZAGUISANTES   2
#define NUEZ             3
#define LANZAHIELO       4

#define ZOMBIE           10   //Zombies
#define FLAG_ZOMBIE      11
#define GIGANTE          12

#define GUISANTE         20   
#define HIELO            21

#define CASA             30
#define PALA             40

#define JUGANDO          0
#define VICTORIA         1
#define DERROTA          2

void iniciar_logica(struct configuracion *cfg);  //Con esto creamos el mapa de juego

int plantar(int tipo, int fila, int col);
int quitar_planta(int fila, int col);

void actualizar_juego(void);

int obtener_celda(int fila, int col);
int obtener_guisante(int fila, int col);
int obtener_dano(int fila, int col);
int obtener_soles_caidos(void);
int estado_juego(void);
int obtener_soles(void);
int obtener_vida_casa(void);
int obtener_oleada(void);
int costo_planta(int tipo);
int obtener_hielo(int fila, int col);
int obtener_congelado(int fila, int col);
int obtener_tumbas(int fila, int col);
int obtener_puntaje(void);
int obtener_record(void);
void guardar_record(void);


bool hay_zombie(void);
bool hay_zombie_comiendo(void);

void cargar_partida(void); // la funcion de apretar l para cargar enta en la ultima linea de logica c

#endif
