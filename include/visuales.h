#ifndef VISUALES_H
#define VISUALES_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdbool.h>

#define NOMBRE_VENTANA  "pvz"

#define ANCHO 800.0f
#define ALTO 600.0f

#define FILAS 5
#define COLUMNAS 11

#define MARGEN_X -31.0f
#define MARGEN_Y 86.0f
#define ANCHO_CELDA 79.0f
#define ALTO_CELDA 97.0f

#define POS_CAMARA_JUGANDO      210.0f
#define VEL_CAMARA              6.0f   

// Funciones de inicio y cierre
bool inicializarVisuales(void);
void cerrarSDL(void);

// Funciones para cargar y dibujar objetos
SDL_Texture *cargarTextura(const char *archivo);
void prepararFondo(void); // Pinta de gris la pantalla antes de dibujar
void RenderizarFondo(SDL_Renderer*, SDL_Texture*, float);
void presentarPantalla(void); // Actualiza la pantalla

//Prueba de la cuadrícula
void RenderizarCuadriculaDebug(SDL_Renderer* renderer);

void loopVisuales(float);

bool animacionInicio(float*);

void imprimirTextura(int, SDL_FRect);

#endif