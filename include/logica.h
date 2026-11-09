#ifndef LOGICA_H
#define LOGICA_H

#include "entradas.h"

#define FILAS_JUEGO      5    //Filas y columnas donde se jugará
#define COLUMNAS_JUEGO   10

#define PASTO            0
#define GIRASOL          1    //Plantas
#define LANZAGUISANTES   2
#define NUEZ             3

#define ZOMBIE           10   //Zombies
#define FLAG_ZOMBIE      11

#define GUISANTE         20   

#define CASA             30

#define JUGANDO          0
#define VICTORIA         1
#define DERROTA          2

void iniciar_logica(struct configuracion *cfg);  //Con esto creamos el mapa de juego

int plantar(int tipo, int fila, int col);

void actualizar_juego(void);

int obtener_celda(int fila, int col);

int estado_juego(void);
int obtener_soles(void);
int obtener_vida_casa(void);
int obtener_oleada(void);
int obtener_total_oleadas(void);
int costo_planta(int tipo);

#endif
