#ifndef VISUALES_H
#define VISUALES_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

#define FILAS 5
#define COLUMNAS 8
#define TAM_CASILLA 64

// Funciones de inicio y cierre
bool inicializarVisuales(void);
void cerrarVisuales(void);

// Funciones para cargar y dibujar objetos
SDL_Texture *cargarTextura(const char *archivo);
void prepararFondo(void); // Pinta de gris la pantalla antes de dibujar
void presentarPantalla(void); // Actualiza la pantalla

// Función que dibuja una textura directo en una coordenadade la matriz
void dibujarCasilla(SDL_Texture *textura, int fila, int columna);

#endif