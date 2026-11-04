/*
 *  MENÚ DE LA LÓGICA
 */

#ifndef LOGICA_H
#define LOGICA_H

//NUMEROS DE ELEMENTOS

// Plantas (0xx) 
#define GIRASOL          1
#define LANZAGUISANTES   2
#define NUEZ             3

// Zombies (1xx) 
#define ZOMBIE           101
#define FLAG_ZOMBIE      102

// Escenario (2xx) 
#define PASTO            200
#define LAPIDA           201
#define CASA             202   //la columna 0 siempre es la casa 

// Proyectiles (3xx) 
#define GUISANTE         300

//estado de la partida q devolverá (estado_juego) 
#define JUGANDO          0
#define VICTORIA         1
#define DERROTA          2

//FUNCIONES

/* se llama una vez para crear la matriz con la 1ra columna de casa
y rellena de pasto */
void iniciar_logica(int **mapa, int filas, int columnas);

// para liberar la memoria 
void liberar_logica(void);

// El jugador quiere plantar. Devuelve 1 si pudo y 0 si no x pocos soles o fuera de la matriz 
int plantar(int **mapa, int tipo, int fila, int col);

// avanza de a un turno se llama una vez por cada vuelta del loop. 
void actualizar_juego(int **mapa);

// Datos para que después en tarea 3 se muestre en pantalla. 
int estado_juego(void);       // JUGANDO, VICTORIA DERROTA
int obtener_soles(void);
int obtener_vida_casa(void);
int obtener_oleada(void);
int obtener_total_oleadas(void);
int costo_planta(int tipo);

#endif
